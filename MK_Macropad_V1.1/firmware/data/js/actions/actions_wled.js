// actions_wled.js
// --------------------------------------------------
// Actions ESP32 / WLED (IDs simples, compatibles firmware)
// --------------------------------------------------

export const WLED_ACTIONS = [

    // -------------------------
    // Actions générales
    // -------------------------
    { id: "wled_on",      label: "Allumer",        icon: "💡", category: "wled", i18n: "wled_on" },
    { id: "wled_off",     label: "Éteindre",       icon: "🌑", category: "wled", i18n: "wled_off" },
    { id: "wled_toggle",  label: "Toggle",         icon: "🔁", category: "wled", i18n: "wled_toggle" },

    // -------------------------
    // Presets WLED
    // -------------------------
    { id: "wled_preset_1", label: "Preset 1", icon: "🎨", category: "wled", i18n: "wled_preset_1" },
    { id: "wled_preset_2", label: "Preset 2", icon: "🌈", category: "wled", i18n: "wled_preset_2" },
    { id: "wled_preset_3", label: "Preset 3", icon: "🔥", category: "wled", i18n: "wled_preset_3" },

    // -------------------------
    // Couleurs directes
    // -------------------------
    { id: "wled_red",    label: "Rouge",    icon: "🟥", category: "wled", i18n: "wled_red" },
    { id: "wled_green",  label: "Vert",     icon: "🟩", category: "wled", i18n: "wled_green" },
    { id: "wled_blue",   label: "Bleu",     icon: "🟦", category: "wled", i18n: "wled_blue" },

    // -------------------------
    // Animations
    // -------------------------
    { id: "wled_rainbow", label: "Arc-en-ciel", icon: "🌈", category: "wled", i18n: "wled_rainbow" },
    { id: "wled_pulse",   label: "Pulse",       icon: "💓", category: "wled", i18n: "wled_pulse" },
    { id: "wled_wave",    label: "Vague",       icon: "🌊", category: "wled", i18n: "wled_wave" }
];
