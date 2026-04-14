// actions_path.js
// --------------------------------------------------
// Action : Lancer une application
// --------------------------------------------------

/**
 * Génère un nom propre automatiquement depuis un chemin
 * Exemple :
 *   C:\Program Files\Blender\blender.exe → Blender
 */
export function generateAppName(path) {
    if (!path) return "Application";

    const parts = path.split(/[/\\]/);
    let file = parts[parts.length - 1];

    const dot = file.lastIndexOf(".");
    if (dot > 0) file = file.substring(0, dot);

    file = file.replace(/[-_]+/g, " ");

    return file
        .split(" ")
        .filter(Boolean)
        .map(w => w.charAt(0).toUpperCase() + w.slice(1))
        .join(" ");
}

/**
 * Rendu de la section "Chemin d'application"
 */
export function renderPathSection(existing) {
    const value = existing?.appPath || "";
    return `
        <h3 data-i18n="action_app_path_title">Chemin d'application</h3>

        <input 
            type="text" 
            id="appPath" 
            class="input"
            placeholder="Ex : C:\\Program Files\\App.exe"
            data-i18n-placeholder="action_app_path_placeholder"
            value="${value}"
        >
    `;
}

/**
 * Récupération de la valeur finale
 */
export function getPathValue(root) {
    const input = root.querySelector("#appPath");
    return input ? input.value.trim() : "";
}

/**
 * 🔥 Fonction ajoutée : reset propre du champ chemin
 */
export function clearPathInput(root) {
    const input = root.querySelector("#appPath");
    if (input) input.value = "";
}
