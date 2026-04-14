import { addBubble, getBubbleValues } from "./actions_bubbles.js";

export const HID_KEYS = [
    "CTRL","ALT","SHIFT","WIN",
    ..."ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    ..."0123456789",
    "ENTER","ESC","TAB","SPACE",
    "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12",
    "-","_","=","+","[","]","{","}","\\","|",
    ";",":","'","\"",",",".","/","<",">","?","~","`"
];

export function renderTagsSection(existing) {
    return `
        <input 
            type="text" 
            id="shortcutSearch" 
            class="input" 
            placeholder="Rechercher une touche…" 
            data-i18n-placeholder="action_search_key"
        >
        <div id="shortcutList" class="shortcut-list"></div>
    `;
}

export function initTagsSection(root, existing, bubbleContainer) {

    const search = root.querySelector("#shortcutSearch");
    const list = root.querySelector("#shortcutList");

    const allKeys = [
        ...HID_KEYS.map(k => ({ label: k })),
        ...(window.currentTags || [])
    ];

    // Charger les bulles existantes
    if (existing?.sequence) {
        existing.sequence.forEach(k => addBubble(bubbleContainer, k));
    }

    function renderList(filter = "") {
        list.innerHTML = allKeys
            .filter(k => k.label.toLowerCase().includes(filter.toLowerCase()))
            .map(k => `<div class="shortcut-item" data-key="${k.label}">${k.label}</div>`)
            .join("");

        list.querySelectorAll(".shortcut-item").forEach(item => {
            item.addEventListener("click", () => {
                const key = item.dataset.key;

                const modifiers = ["CTRL", "ALT", "SHIFT", "WIN"];
                const current = getBubbleValues(bubbleContainer);

                // Si on clique un modificateur → on l'ajoute simplement
                if (modifiers.includes(key)) {
                    addBubble(bubbleContainer, key);
                    return;
                }

                // Si on clique une touche normale et que la dernière bulle contient un modificateur → fusion
                const last = current[current.length - 1];

                if (last && modifiers.some(m => last.includes(m))) {
                    // Fusion WIN + R → WIN+R
                    const merged = last + "+" + key;

                    // On remplace la dernière bulle par la fusion
                    bubbleContainer.lastChild.remove();
                    addBubble(bubbleContainer, merged);
                    return;
                }

                // Sinon → comportement normal
                addBubble(bubbleContainer, key);
            });
        });
    }

    renderList();
    search.addEventListener("input", () => renderList(search.value));

    return () => getBubbleValues(bubbleContainer);
}
