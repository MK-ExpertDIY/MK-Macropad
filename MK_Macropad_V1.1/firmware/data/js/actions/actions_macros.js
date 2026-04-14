// actions_macros.js
// --------------------------------------------------
// Macros prédéfinies : icônes + labels + catégories
// --------------------------------------------------
//
// IMPORTANT :
// Ce fichier NE GÈRE PAS les bulles.
// Les bulles affichent macro.label (lisible).
// Le firmware reçoit macro.id (technique).
//
// --------------------------------------------------

export const PRESET_MACROS = [

    // -------------------------
    // 🌐 WEB
    // -------------------------
    { id: "open_youtube",   label: "YouTube",   icon: "🎬", category: "web", i18n: "macro_youtube" },
    { id: "open_google",    label: "Google",    icon: "🔍", category: "web", i18n: "macro_google" },
    { id: "open_reddit",    label: "Reddit",    icon: "👽", category: "web", i18n: "macro_reddit" },
    { id: "open_facebook",  label: "Facebook",  icon: "📘", category: "web", i18n: "macro_facebook" },
    { id: "open_instagram", label: "Instagram", icon: "📸", category: "web", i18n: "macro_instagram" },
    { id: "open_wikipedia", label: "Wikipedia", icon: "📚", category: "web", i18n: "macro_wikipedia" },
    { id: "open_chatgpt",   label: "ChatGPT",   icon: "🤖", category: "web", i18n: "macro_chatgpt" },
    { id: "open_amazon",    label: "Amazon",    icon: "🛒", category: "web", i18n: "macro_amazon" },
    { id: "open_lbc",       label: "LeBonCoin", icon: "🏷️", category: "web", i18n: "macro_lbc" },
    { id: "open_github",    label: "GitHub",    icon: "🐙", category: "web", i18n: "macro_github" },

    // -------------------------
    // 🖥️ APPLICATIONS
    // -------------------------
    { id: "open_notepad",       label: "Bloc-notes",                 icon: "📝", category: "apps", i18n: "macro_notepad" },
    { id: "open_calc",          label: "Calculatrice",               icon: "🧮", category: "apps", i18n: "macro_calc" },
    { id: "open_explorer",      label: "Explorateur",                icon: "📁", category: "apps", i18n: "macro_explorer" },
    { id: "open_powershell",    label: "PowerShell",                 icon: "🖥️", category: "apps", i18n: "macro_powershell" },
    { id: "open_cmd",           label: "CMD",                        icon: "💻", category: "apps", i18n: "macro_cmd" },
    { id: "open_devicemanager", label: "Gestionnaire périphériques", icon: "🛠️", category: "apps", i18n: "macro_devicemanager" },
    { id: "open_services",      label: "Services Windows",           icon: "⚙️", category: "apps", i18n: "macro_services" },
    { id: "open_regedit",       label: "Regedit",                    icon: "🧰", category: "apps", i18n: "macro_regedit" },

    // -------------------------
    // ✂️ RACCOURCIS
    // -------------------------
    { id: "copy",        label: "Copier",            icon: "📋", category: "shortcuts", i18n: "macro_copy" },
    { id: "paste",       label: "Coller",            icon: "📥", category: "shortcuts", i18n: "macro_paste" },
    { id: "cut",         label: "Couper",            icon: "✂️", category: "shortcuts", i18n: "macro_cut" },
    { id: "select_all",  label: "Tout sélectionner", icon: "🔲", category: "shortcuts", i18n: "macro_select_all" },
    { id: "undo",        label: "Annuler",           icon: "↩️", category: "shortcuts", i18n: "macro_undo" },
    { id: "redo",        label: "Rétablir",          icon: "↪️", category: "shortcuts", i18n: "macro_redo" },
    { id: "save",        label: "Sauvegarder",       icon: "💾", category: "shortcuts", i18n: "macro_save" },
    { id: "search",      label: "Rechercher",        icon: "🔍", category: "shortcuts", i18n: "macro_search" },

    // -------------------------
    // 🧩 SYSTÈME
    // -------------------------
    { id: "enter",         label: "Entrer",              icon: "⏎", category: "system", i18n: "macro_enter" },
    { id: "printscreen",   label: "Capture écran",       icon: "📸", category: "system", i18n: "macro_printscreen" },
    { id: "lock",          label: "Verrouiller",         icon: "🔒", category: "system", i18n: "macro_lock" },
    { id: "desktop",       label: "Bureau",              icon: "🪟", category: "system", i18n: "macro_desktop" },
    { id: "switch_window", label: "Changer fenêtre",     icon: "🔄", category: "system", i18n: "macro_switch_window" },
    { id: "close_window",  label: "Fermer fenêtre",      icon: "❌", category: "system", i18n: "macro_close_window" },
    { id: "task_manager",  label: "Gestionnaire tâches", icon: "🧰", category: "system", i18n: "macro_task_manager" }
];
