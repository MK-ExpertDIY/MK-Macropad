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
//  DASHBOARD PRINCIPAL
// ------------------------------------------------------------

import { renderActionsTab, initActionsTab } from "./onglets/onglet_touches.js";
import { renderLedsTab, initLedControls } from "./onglets/onglet_led.js";
import { renderSystemTab, initSystemTab } from "./onglets/onglet_parametres.js";
import { translations, applyTranslation, setLanguage } from "./core/lang.js";

// 🔥 IMPORTANT : langue par défaut AVANT tout
window.currentLang = "fr";

// 🔥 Nouveau : stockage du layout courant
window.currentLayoutCode = null;

// 🔥 Mise à jour du layout + changement de langue
window.updateLayoutDisplay = function(layout) {
    const el = document.getElementById("layoutIndicator");
    if (!el) return;

    const map = {
        "BE": "AZERTY (BE)",
        "FR": "AZERTY (FR)",
        "US": "QWERTY (US)"
    };

    // On mémorise le layout
    window.currentLayoutCode = layout;

    const lang = window.currentLang || "fr";
    const label = translations?.[lang]?.layout_label || "Layout : ";
    const layoutText = map[layout] || layout || "???";

    el.textContent = label + layoutText;

    // 🔥 Auto-switch langue
    if (layout === "US") setLanguage("en");
    else setLanguage("fr");
};

document.addEventListener("DOMContentLoaded", async () => {

    const tabContent = document.getElementById("tab-content");
    const tabButtons = document.querySelectorAll(".tab-btn");

    // Charger layout initial
    fetch("/api/layout")
        .then(r => r.json())
        .then(data => {
            if (data.layout) window.updateLayoutDisplay(data.layout);
        });

    async function loadKeyConfig() {
        try {
            const res = await fetch("/api/keyconfig");
            return await res.json();
        } catch {
            return {};
        }
    }

    async function saveConfig(config) {
        await fetch("/api/keyconfig", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(config)
        });
    }

    function getKeyLabel(cfg) {
        if (!cfg) return "";
        if (cfg.type === "launch_app") return cfg.appName;
        if (cfg.text) return cfg.text;
        if (cfg.sequence?.length) return cfg.sequence.join("+");
        return "";
    }

    function renderKeysGrid(config) {
        return `
            <div class="keys-grid">
                ${Array.from({length: 9}).map((_, i) => {
                    const id = i + 1;
                    const label = getKeyLabel(config[id]);
                    return `
                        <div class="key" data-id="${id}">
                            <div class="key-number" style="${label ? "display:none" : ""}">${id}</div>
                            <div class="key-label">${label}</div>
                        </div>
                    `;
                }).join("")}
            </div>
        `;
    }

    function initKeysEvents(config) {
        document.querySelectorAll(".key").forEach(k => {
            k.addEventListener("click", () => {
                const id = k.dataset.id;
                const existing = config[id] || {};

                document.querySelectorAll(".key.active").forEach(el => el.classList.remove("active"));
                k.classList.add("active");

                const oldPanel = document.querySelector(".action-panel");
                if (oldPanel) oldPanel.remove();

                tabContent.insertAdjacentHTML("beforeend", renderActionsTab(id, existing));

                initActionsTab(id, existing, tabContent, async (updated) => {

                    config[id] = updated;
                    await saveConfig(config);

                    tabContent.innerHTML = renderKeysGrid(config);
                    initKeysEvents(config);
                    applyTranslation(window.currentLang);
                });

                applyTranslation(window.currentLang);
            });
        });
    }

    async function showKeysTab() {
        const config = await loadKeyConfig();
        tabContent.innerHTML = renderKeysGrid(config);
        initKeysEvents(config);
        applyTranslation(window.currentLang);
    }

    function showLedsTab() {
        tabContent.innerHTML = renderLedsTab();
        initLedControls(tabContent);
        applyTranslation(window.currentLang);
    }

    function showSystemTab() {
        tabContent.innerHTML = renderSystemTab();
        initSystemTab(tabContent);
        applyTranslation(window.currentLang);
    }

    tabButtons.forEach(btn => {
        btn.addEventListener("click", () => {

            document.querySelector(".tab-btn.active")?.classList.remove("active");
            btn.classList.add("active");

            switch (btn.dataset.tab) {
                case "keys": showKeysTab(); break;
                case "leds": showLedsTab(); break;
                case "system": showSystemTab(); break;
            }
        });
    });

    showKeysTab();
});
