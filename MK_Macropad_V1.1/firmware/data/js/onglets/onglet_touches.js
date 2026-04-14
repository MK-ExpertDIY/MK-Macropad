/*
 * MK Macropad - Firmware / User Interface
 * Copyright (C) 2026 MK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */



// ------------------------------------------------------------
//  ONGLET TOUCHES — CONFIGURATION DES ACTIONS
// ------------------------------------------------------------

import { renderTextSection, initTextSection, getTextValue } from "../actions/actions_text.js";
import { renderTagsSection, initTagsSection } from "../actions/actions_tags.js";
import { PRESET_MACROS } from "../actions/actions_macros.js";
import { renderPathSection, getPathValue, generateAppName, clearPathInput } from "../actions/actions_path.js";
import { addBubble, getBubbleValues, clearBubbles } from "../actions/actions_bubbles.js";

/* ---------------------------------------------------------
   🔥 Groupes de macros prédéfinies
   --------------------------------------------------------- */
function groupPresets() {
    return {
        web: PRESET_MACROS.filter(m => m.category === "web"),
        apps: PRESET_MACROS.filter(m => m.category === "apps"),
        shortcuts: PRESET_MACROS.filter(m => m.category === "shortcuts"),
        system: PRESET_MACROS.filter(m => m.category === "system")
    };
}

/* ---------------------------------------------------------
   🔥 Fusion intelligente des touches HID
   --------------------------------------------------------- */
function mergeComboTags(tags) {
    if (!tags || tags.length === 0) return [];

    const modifiers = ["CTRL", "ALT", "SHIFT", "WIN"];
    let buffer = [];
    let result = [];

    for (const t of tags) {
        if (modifiers.includes(t)) {
            buffer.push(t);
        } else {
            if (buffer.length > 0) {
                result.push(buffer.join("+") + "+" + t);
                buffer = [];
            } else {
                result.push(t);
            }
        }
    }

    if (buffer.length > 0) {
        result.push(buffer.join("+"));
    }

    return result;
}

/* ---------------------------------------------------------
   🔥 Rendu complet du panneau d’action
   --------------------------------------------------------- */
export function renderActionsTab(id, existing) {
    const groups = groupPresets();

    return `
        <div class="section action-panel">

            <h2 data-i18n="action_configure_key">Configurer la touche ${id}</h2>

            <div id="actionBubbles" style="margin:10px 0;"></div>

            <h3 data-i18n="action_custom_shortcut">Raccourci personnalisé</h3>
            <div id="tagsSection">
                ${renderTagsSection(existing)}
            </div>

            <h3 data-i18n="action_presets">Raccourcis prédéfinis</h3>

            <select id="presetSelect" class="input">

                <option value="" selected disabled data-i18n="action_choose_macro">Choisir une macro…</option>

                <!-- 🌐 WEB -->
                <optgroup label="🌐 Web" data-i18n="action_group_web">
                    ${groups.web.map(m => `
                        <option value="${m.id}" data-i18n="${m.i18n}" data-icon="${m.icon}">
                            ${m.icon} ${m.label}
                        </option>
                    `).join("")}
                </optgroup>

                <!-- 🖥️ APPS -->
                <optgroup label="🖥️ Applications" data-i18n="action_group_apps">
                    ${groups.apps.map(m => `
                        <option value="${m.id}" data-i18n="${m.i18n}" data-icon="${m.icon}">
                            ${m.icon} ${m.label}
                        </option>
                    `).join("")}
                </optgroup>

                <!-- ✂️ RACCOURCIS -->
                <optgroup label="✂️ Raccourcis" data-i18n="action_group_shortcuts">
                    ${groups.shortcuts.map(m => `
                        <option value="${m.id}" data-i18n="${m.i18n}" data-icon="${m.icon}">
                            ${m.icon} ${m.label}
                        </option>
                    `).join("")}
                </optgroup>

                <!-- 🧩 SYSTEME -->
                <optgroup label="🧩 Système" data-i18n="action_group_system">
                    ${groups.system.map(m => `
                        <option value="${m.id}" data-i18n="${m.i18n}" data-icon="${m.icon}">
                            ${m.icon} ${m.label}
                        </option>
                    `).join("")}
                </optgroup>

            </select>

            ${renderPathSection(existing)}

            <div id="textSection">
                ${renderTextSection(existing)}
            </div>

            <div class="buttons">
                <button class="btn btn-primary" id="saveKey" data-i18n="action_save">Enregistrer</button>
            </div>
        </div>
    `;
}

/* ---------------------------------------------------------
   🔥 Initialisation du panneau d’action
   --------------------------------------------------------- */
export function initActionsTab(id, existing, root, onSave) {

    const actionBubbles = root.querySelector("#actionBubbles");

    const getTagsSequence = initTagsSection(root, existing, actionBubbles);
    initTextSection(root, existing);

    /* ---------------------------------------------------------
       RESET GLOBAL
       --------------------------------------------------------- */
    function resetAll() {
        clearBubbles(actionBubbles);
        clearPathInput(root);

        const textInput = root.querySelector("#textInput");
        if (textInput) textInput.value = "";

        const tagsSection = root.querySelector("#tagsSection");
        if (tagsSection) tagsSection.innerHTML = renderTagsSection({ sequence: [] });
    }

    /* ---------------------------------------------------------
       Si action = launch_app
       --------------------------------------------------------- */
    if (existing?.type === "launch_app") {
        clearBubbles(actionBubbles);
        addBubble(actionBubbles, existing.appName, existing.appPath);
        const appPathEl = root.querySelector("#appPath");
        if (appPathEl) appPathEl.value = existing.appPath;
    }

    /* ---------------------------------------------------------
       Sélection d’une macro prédéfinie
       --------------------------------------------------------- */
    const presetSelect = root.querySelector("#presetSelect");
    if (presetSelect) {
        presetSelect.addEventListener("change", () => {
            const id = presetSelect.value;
            const macro = PRESET_MACROS.find(m => m.id === id);
            if (!macro) return;

            resetAll();
            addBubble(actionBubbles, macro.label, macro.label);
        });
    }

    /* ---------------------------------------------------------
       Texte libre → bulle
       --------------------------------------------------------- */
    const textInput = root.querySelector("#textInput");
    if (textInput) {
        textInput.addEventListener("change", () => {
            const value = textInput.value.trim();
            if (!value) return;

            resetAll();
            addBubble(actionBubbles, value, value);
        });
    }

    /* ---------------------------------------------------------
       Chemin d’application → bulle
       --------------------------------------------------------- */
    const appPathInput = root.querySelector("#appPath");
    if (appPathInput) {
        appPathInput.addEventListener("input", () => {

            clearBubbles(actionBubbles);
            const textEl = root.querySelector("#textInput");
            if (textEl) textEl.value = "";
            const tagsSection = root.querySelector("#tagsSection");
            if (tagsSection) tagsSection.innerHTML = renderTagsSection({ sequence: [] });

            const value = appPathInput.value.trim();
            if (value.length > 0) {
                const appName = generateAppName(value);
                addBubble(actionBubbles, appName, value);
            }
        });
    }

    /* ---------------------------------------------------------
       Sauvegarde
       --------------------------------------------------------- */
    const saveBtn = root.querySelector("#saveKey");
    if (saveBtn) {
        saveBtn.onclick = () => {

            const path = getPathValue(root);

            // 1 — Lancement d’application
            if (path) {
                const appName = generateAppName(path);

                const updated = {
                    type: "launch_app",
                    appPath: path,
                    appName: appName
                };

                clearBubbles(actionBubbles);
                addBubble(actionBubbles, appName, path);

                onSave(updated);
                return;
            }

            // 2 — Macro prédéfinie ou texte libre
            const sequencePreset = getBubbleValues(actionBubbles) || [];
            const text = getTextValue(root);

            if (sequencePreset.length > 0) {
                onSave({
                    type: "sequence",
                    text: text || "",
                    sequence: sequencePreset
                });
                return;
            }

            // 3 — Raccourci HID
            const rawTags = getTagsSequence();
            const merged = mergeComboTags(rawTags);

            onSave({
                type: "sequence",
                text: text || "",
                sequence: merged
            });
        };
    }
}
