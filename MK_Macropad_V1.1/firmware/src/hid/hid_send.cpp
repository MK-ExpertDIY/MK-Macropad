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



#include "hid_send.h"
#include "ui/keytags_be.h"
#include "../globals.h"
#include <ctype.h>

extern BleKeyboard bleKeyboard;

/*
 * Vérifie si BLE est connecté
 */
bool ensureBleConnected() {
    if (!bleKeyboard.isConnected()) {
        Serial.println("[HID] BLE non connecté → envoi annulé");
        return false;
    }
    return true;
}

/*
 * sendChar(c)
 * Envoie un caractère Unicode → HID (layout dynamique)
 */
void sendChar(uint32_t c) {
    if (!ensureBleConnected()) return;

    KeyInfo k = getKey(c);   // 🔥 dynamique

    uint8_t modifiers = 0;
    if (k.shift) modifiers |= 0x02;
    if (k.altgr) modifiers |= 0x40;

    uint8_t report[8] = { modifiers, 0, k.hid, 0, 0, 0, 0, 0 };
    bleKeyboard.sendRawReport(report, 8);
    delay(2);

    uint8_t empty[8] = {0};
    bleKeyboard.sendRawReport(empty, 8);
    delay(1);
}

/*
 * sendString(text)
 * Envoie une chaîne caractère par caractère.
 */
void sendString(const char* text) {
    if (!ensureBleConnected()) return;

    while (*text) {
        sendChar((uint8_t)*text);
        delay(2);
        text++;
    }
}

/*
 * runMacro(macro)
 * Moteur HID unifié
 */
void runMacro(const String& macro) {

    if (!ensureBleConnected()) return;

    // -------------------------
    // 1. Touches spéciales simples
    // -------------------------
    if (macro == "ENTER") {
        uint8_t report[8] = {0, 0, 40, 0, 0, 0, 0, 0};
        bleKeyboard.sendRawReport(report, 8);
        delay(8);
        uint8_t empty[8] = {0};
        bleKeyboard.sendRawReport(empty, 8);
        return;
    }

    if (macro == "TAB") {
        uint8_t report[8] = {0, 0, 43, 0, 0, 0, 0, 0};
        bleKeyboard.sendRawReport(report, 8);
        delay(8);
        uint8_t empty[8] = {0};
        bleKeyboard.sendRawReport(empty, 8);
        return;
    }

    if (macro == "ESC") {
        uint8_t report[8] = {0, 0, 0x29, 0, 0, 0, 0, 0};
        bleKeyboard.sendRawReport(report, 8);
        delay(8);
        uint8_t empty[8] = {0};
        bleKeyboard.sendRawReport(empty, 8);
        return;
    }

    // -------------------------
    // 2. Modificateurs seuls
    // -------------------------
    if (macro == "CTRL")   { uint8_t r[8]={0x01}; bleKeyboard.sendRawReport(r,8); delay(8); uint8_t e[8]={0}; bleKeyboard.sendRawReport(e,8); return; }
    if (macro == "SHIFT")  { uint8_t r[8]={0x02}; bleKeyboard.sendRawReport(r,8); delay(8); uint8_t e[8]={0}; bleKeyboard.sendRawReport(e,8); return; }
    if (macro == "ALT")    { uint8_t r[8]={0x04}; bleKeyboard.sendRawReport(r,8); delay(8); uint8_t e[8]={0}; bleKeyboard.sendRawReport(e,8); return; }
    if (macro == "WIN")    { uint8_t r[8]={0x08}; bleKeyboard.sendRawReport(r,8); delay(8); uint8_t e[8]={0}; bleKeyboard.sendRawReport(e,8); return; }
    if (macro == "ALTGR")  { uint8_t r[8]={0x40}; bleKeyboard.sendRawReport(r,8); delay(8); uint8_t e[8]={0}; bleKeyboard.sendRawReport(e,8); return; }

    // -------------------------
    // 3. WIN + lettre
    // -------------------------
    if (macro.startsWith("WIN+") && macro.length() == 5) {
        char c = tolower((unsigned char)macro[4]);
        KeyInfo k = getKey(c);   // 🔥 dynamique

        uint8_t mods = 0x08;
        if (k.shift) mods |= 0x02;
        if (k.altgr) mods |= 0x40;

        uint8_t report[8] = {mods, 0, k.hid, 0, 0, 0, 0, 0};
        bleKeyboard.sendRawReport(report, 8);
        delay(8);
        uint8_t empty[8]={0};
        bleKeyboard.sendRawReport(empty,8);
        return;
    }

    // -------------------------
    // 4. Lettres simples
    // -------------------------
    if (macro.length() == 1) {
        sendChar((uint8_t)macro[0]);
        return;
    }

    // -------------------------
    // 5. F1–F12
    // -------------------------
    const struct { const char* name; uint8_t hid; } fnKeys[] = {
        {"F1",0x3A},{"F2",0x3B},{"F3",0x3C},{"F4",0x3D},{"F5",0x3E},{"F6",0x3F},
        {"F7",0x40},{"F8",0x41},{"F9",0x42},{"F10",0x43},{"F11",0x44},{"F12",0x45}
    };

    for (auto &k : fnKeys) {
        if (macro == k.name) {
            uint8_t report[8] = {0, 0, k.hid, 0, 0, 0, 0, 0};
            bleKeyboard.sendRawReport(report, 8);
            delay(8);
            uint8_t empty[8]={0};
            bleKeyboard.sendRawReport(empty,8);
            return;
        }
    }

    // -------------------------
    // 6. Combinaisons multi-modificateurs
    // -------------------------
    if (macro.indexOf('+') != -1) {

        std::vector<String> parts;
        int start = 0;
        while (true) {
            int idx = macro.indexOf('+', start);
            if (idx == -1) { parts.push_back(macro.substring(start)); break; }
            parts.push_back(macro.substring(start, idx));
            start = idx + 1;
        }

        uint8_t mods = 0;
        for (size_t i = 0; i < parts.size() - 1; i++) {
            if (parts[i] == "CTRL")  mods |= 0x01;
            if (parts[i] == "SHIFT") mods |= 0x02;
            if (parts[i] == "ALT")   mods |= 0x04;
            if (parts[i] == "WIN")   mods |= 0x08;
            if (parts[i] == "ALTGR") mods |= 0x40;
        }

        String key = parts.back();

        for (auto &k : fnKeys) {
            if (key == k.name) {
                uint8_t report[8] = {mods, 0, k.hid, 0, 0, 0, 0, 0};
                bleKeyboard.sendRawReport(report, 8);
                delay(8);
                uint8_t empty[8]={0};
                bleKeyboard.sendRawReport(empty,8);
                return;
            }
        }

        if (key == "ESC") {
            uint8_t report[8] = {mods, 0, 0x29, 0, 0, 0, 0, 0};
            bleKeyboard.sendRawReport(report, 8);
            delay(8);
            uint8_t empty[8]={0};
            bleKeyboard.sendRawReport(empty,8);
            return;
        }

        char c = tolower((unsigned char)key[0]);
        KeyInfo k = getKey(c);   // 🔥 dynamique

        if (k.shift) mods |= 0x02;
        if (k.altgr) mods |= 0x40;

        uint8_t report[8] = {mods, 0, k.hid, 0, 0, 0, 0, 0};
        bleKeyboard.sendRawReport(report, 8);
        delay(8);
        uint8_t empty[8]={0};
        bleKeyboard.sendRawReport(empty,8);
        return;
    }
}
