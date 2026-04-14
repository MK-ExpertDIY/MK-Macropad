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




#include "macros.h"
#include "../hid/hid_send.h"   // runMacro(), sendString(), sendChar()

// --------------------------------------------------
// Mapping LABEL (UI) → ID technique (firmware)
// --------------------------------------------------
String mapLabelToId(const String& label) {

    // 🌐 WEB
    if (label == "YouTube")        return "open_youtube";
    if (label == "Google")         return "open_google";
    if (label == "Reddit")         return "open_reddit";
    if (label == "Facebook")       return "open_facebook";
    if (label == "Instagram")      return "open_instagram";
    if (label == "Wikipedia")      return "open_wikipedia";
    if (label == "ChatGPT")        return "open_chatgpt";
    if (label == "Amazon")         return "open_amazon";
    if (label == "LeBonCoin")      return "open_lbc";
    if (label == "GitHub")         return "open_github";

    // 🖥️ APPLICATIONS
    if (label == "Bloc-notes")                 return "open_notepad";
    if (label == "Calculatrice")               return "open_calc";
    if (label == "Explorateur")                return "open_explorer";
    if (label == "PowerShell")                 return "open_powershell";
    if (label == "CMD")                        return "open_cmd";
    if (label == "Gestionnaire périphériques") return "open_devicemanager";
    if (label == "Services Windows")           return "open_services";
    if (label == "Regedit")                    return "open_regedit";

    // ✂️ RACCOURCIS
    if (label == "Copier")            return "copy";
    if (label == "Coller")            return "paste";
    if (label == "Couper")            return "cut";
    if (label == "Tout sélectionner") return "select_all";
    if (label == "Annuler")           return "undo";
    if (label == "Rétablir")          return "redo";
    if (label == "Sauvegarder")       return "save";
    if (label == "Rechercher")        return "search";

    // 🧩 SYSTÈME
    if (label == "Entrer")            return "enter";
    if (label == "Capture écran")     return "printscreen";
    if (label == "Verrouiller")       return "lock";
    if (label == "Bureau")            return "desktop";
    if (label == "Changer fenêtre")   return "switch_window";
    if (label == "Fermer fenêtre")    return "close_window";
    if (label == "Gestionnaire tâches") return "task_manager";

    // Fallback : HID brut (CTRL, ALT, A, F5…)
    return label;
}

// --------------------------------------------------
// Routeur principal des macros prédéfinies
// --------------------------------------------------
void runMacroBE(const String& id) {

    // -------------------------
    // 🌐 WEB
    // -------------------------
    if (id == "open_youtube") {
        runMacro("WIN+R"); delay(200);
        sendString("https://youtube.com");
        runMacro("ENTER");
    }
    else if (id == "open_google") {
        runMacro("WIN+R"); delay(200);
        sendString("https://google.com");
        runMacro("ENTER");
    }
    else if (id == "open_reddit") {
        runMacro("WIN+R"); delay(200);
        sendString("https://reddit.com");
        runMacro("ENTER");
    }
    else if (id == "open_facebook") {
        runMacro("WIN+R"); delay(200);
        sendString("https://facebook.com");
        runMacro("ENTER");
    }
    else if (id == "open_instagram") {
        runMacro("WIN+R"); delay(200);
        sendString("https://instagram.com");
        runMacro("ENTER");
    }
    else if (id == "open_wikipedia") {
        runMacro("WIN+R"); delay(200);
        sendString("https://wikipedia.org");
        runMacro("ENTER");
    }
    else if (id == "open_chatgpt") {
        runMacro("WIN+R"); delay(200);
        sendString("https://chat.openai.com");
        runMacro("ENTER");
    }
    else if (id == "open_amazon") {
        runMacro("WIN+R"); delay(200);
        sendString("https://amazon.fr");
        runMacro("ENTER");
    }
    else if (id == "open_lbc") {
        runMacro("WIN+R"); delay(200);
        sendString("https://leboncoin.fr");
        runMacro("ENTER");
    }
    else if (id == "open_github") {
        runMacro("WIN+R"); delay(200);
        sendString("https://github.com");
        runMacro("ENTER");
    }

    // -------------------------
    // 🖥️ APPLICATIONS
    // -------------------------
    else if (id == "open_notepad") {
        runMacro("WIN+R"); delay(200);
        sendString("notepad");
        runMacro("ENTER");
    }
    else if (id == "open_calc") {
        runMacro("WIN+R"); delay(200);
        sendString("calc");
        runMacro("ENTER");
    }
    else if (id == "open_explorer") {
        runMacro("WIN+E");
    }
    else if (id == "open_powershell") {
        runMacro("WIN+R"); delay(200);
        sendString("powershell");
        runMacro("ENTER");
    }
    else if (id == "open_cmd") {
        runMacro("WIN+R"); delay(200);
        sendString("cmd");
        runMacro("ENTER");
    }
    else if (id == "open_devicemanager") {
        runMacro("WIN+X"); delay(200);
        runMacro("M");
    }
    else if (id == "open_services") {
        runMacro("WIN+R"); delay(200);
        sendString("services.msc");
        runMacro("ENTER");
    }
    else if (id == "open_regedit") {
        runMacro("WIN+R"); delay(200);
        sendString("regedit");
        runMacro("ENTER");
    }

    // -------------------------
    // ✂️ RACCOURCIS
    // -------------------------
    else if (id == "copy")        runMacro("CTRL+C");
    else if (id == "paste")       runMacro("CTRL+V");
    else if (id == "cut")         runMacro("CTRL+X");
    else if (id == "select_all")  runMacro("CTRL+A");
    else if (id == "undo")        runMacro("CTRL+Z");
    else if (id == "redo")        runMacro("CTRL+SHIFT+Z");
    else if (id == "save")        runMacro("CTRL+S");
    else if (id == "search")      runMacro("CTRL+F");

    // -------------------------
    // 🧩 SYSTÈME
    // -------------------------
    else if (id == "enter")         runMacro("ENTER");
    else if (id == "printscreen")   runMacro("PRINTSCREEN");
    else if (id == "lock")          runMacro("WIN+L");
    else if (id == "desktop")       runMacro("WIN+D");
    else if (id == "switch_window") runMacro("ALT+TAB");
    else if (id == "close_window")  runMacro("ALT+F4");
    else if (id == "task_manager")  runMacro("CTRL+SHIFT+ESC");
}
