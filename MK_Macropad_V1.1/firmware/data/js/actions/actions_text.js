// actions_text.js
// --------------------------------------------------
// ZONE STABLE : LOGIQUE TEXTE À ENVOYER
// Cette logique est critique pour le projet et NE DOIT PAS être modifiée.
// --------------------------------------------------

// Rendu de la section texte
export function renderTextSection(existing) {
    const value = existing?.text || "";
    return `
        <h3 data-i18n="action_text_title">Texte à envoyer</h3>

        <input 
            type="text" 
            id="textInput" 
            placeholder="Ex : youtube.com"
            data-i18n-placeholder="action_text_placeholder"
            value="${value}"
        >
    `;
}

// Initialisation de la section texte
export function initTextSection(panel, existing) {
    const textInput = panel.querySelector("#textInput");

    // Remplissage initial (déjà géré dans renderTextSection)
    if (existing?.text) {
        textInput.value = existing.text;
    }
}

// Récupération de la valeur finale
export function getTextValue(panel) {
    const textInput = panel.querySelector("#textInput");
    return textInput ? textInput.value.trim() : "";
}
