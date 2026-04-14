// actions_bubbles.js
// --------------------------------------------------
// Gestion des bulles (tags visuels) pour séquences, presets, WLED, etc.
// --------------------------------------------------

/**
 * Crée une bulle visuelle (tag)
 * @param {string} display - Le texte affiché dans la bulle (ex: "YouTube")
 * @param {string|null} realValue - La valeur réelle stockée (ex: "open_youtube")
 * @returns {HTMLElement} - L'élément <span> représentant la bulle
 */
export function createBubble(display, realValue = null) {
    const tag = document.createElement("span");
    tag.className = "bubble-tag";

    // Valeur réelle (ID ou texte brut)
    tag.dataset.value = realValue || display;

    // Texte affiché
    const textEl = document.createElement("span");
    textEl.className = "bubble-text";
    textEl.textContent = display;
    tag.appendChild(textEl);

    // Suppression au clic
    tag.addEventListener("click", () => tag.remove());

    return tag;
}

/**
 * Ajoute une bulle dans un conteneur
 * @param {HTMLElement} container - Le conteneur où ajouter la bulle
 * @param {string} display - Texte affiché
 * @param {string|null} realValue - Valeur réelle stockée
 */
export function addBubble(container, display, realValue = null) {
    const bubble = createBubble(display, realValue);
    container.appendChild(bubble);
}

/**
 * Récupère toutes les valeurs réelles des bulles dans un conteneur
 * @param {HTMLElement} container
 * @returns {string[]}
 */
export function getBubbleValues(container) {
    return [...container.querySelectorAll(".bubble-tag")]
        .map(el => el.dataset.value);
}

/**
 * 🔥 Fonction ajoutée : reset propre des bulles
 * @param {HTMLElement} container
 */
export function clearBubbles(container) {
    if (container) container.innerHTML = "";
}
