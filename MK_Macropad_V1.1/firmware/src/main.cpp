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



#include <Arduino.h>
#include "globals.h"
#include "wifi/webservermanager.h"
#include "hid/hid_send.h"
#include "macros/macros.h"
#include "led/led_engine.h"
#include <Preferences.h>

// ----------------------------------------------------
//  🔥 Gestion du layout clavier
// ----------------------------------------------------

// Layout actif (par défaut BE)
Layout currentLayout = LAYOUT_BE;

// Pointeur vers la fonction de mapping active
KeyInfo (*getKey)(uint32_t c) = getKeyBE;

// Setter propre
void setLayout(Layout l) {
    currentLayout = l;

    switch (l) {
        case LAYOUT_BE:
            getKey = getKeyBE;
            Serial.println("[LAYOUT] BE activé");
            break;

        case LAYOUT_FR:
            getKey = getKeyFR;
            Serial.println("[LAYOUT] FR activé");
            break;

        case LAYOUT_US:
            getKey = getKeyUS;
            Serial.println("[LAYOUT] US activé");
            break;
    }
}

// ----------------------------------------------------
//  Gestion BLE / LEDs
// ----------------------------------------------------
void onBleConnected() {
    if (!LedEngine::getUserPower()) {
        Serial.println("[BLE] Connexion détectée mais LEDs OFF → ignoré");
        return;
    }

    Serial.println("[BLE] Connexion détectée → LEDs ON");

    // On restaure le preset LED sauvegardé (mode + couleur + luminosité)
    LedEngine::restoreState();

    // Et on remonte progressivement vers la dernière luminosité connue
    LedEngine::fadeTo(LedEngine::getLastBrightness(), 1200);
}

void onBleDisconnected() {
    if (!LedEngine::getUserPower()) {
        Serial.println("[BLE] Déconnexion mais LEDs OFF → ignoré");
        NimBLEDevice::startAdvertising();
        return;
    }

    Serial.println("[BLE] Déconnexion → LEDs OFF");
    LedEngine::fadeTo(0, 1200);

    delay(50); // stabilisation BLE
    NimBLEDevice::startAdvertising();

    Serial.println("[BLE] Advertising relancé");
}

// ----------------------------------------------------
//  9 touches → GPIO configurables
// ----------------------------------------------------
#define KEY_COUNT 9

// ----------------------------------------------------
//  Utilitaires HID
// ----------------------------------------------------
void send_text(const String &text) {
    sendString(text.c_str());
}

// ----------------------------------------------------
//  Exécution action touche
// ----------------------------------------------------
void executeKeyAction(const KeyConfig &cfg) {

    if (!bleKeyboard.isConnected()) {
        Serial.println("[ACTION] Ignoré → BLE non connecté");
        return;
    }

    if (cfg.type == "launch_app") {
        Serial.println("[ACTION] launch_app");

        runMacro("WIN+R");
        delay(200);

        if (cfg.appPath.length() > 0) {
            sendString(cfg.appPath.c_str());
            delay(80);
        }

        runMacro("ENTER");
        delay(80);
        return;
    }

    if (!cfg.sequence.empty()) {

        if (cfg.sequence.size() == 1) {

            String value = cfg.sequence[0];
            String id    = mapLabelToId(value);

            bool isPredefined =
                id.startsWith("open_") ||
                id == "copy" || id == "paste" || id == "cut" ||
                id == "select_all" || id == "undo" || id == "redo" ||
                id == "save" || id == "search" ||
                id == "enter" || id == "printscreen" || id == "lock" ||
                id == "desktop" || id == "switch_window" ||
                id == "close_window" || id == "task_manager";

            if (isPredefined) runMacroBE(id);
            else runMacro(id);
        }

        else {
            for (size_t i = 0; i < cfg.sequence.size(); i++) {

                String cmd = cfg.sequence[i];

                if (cmd.startsWith("STRING:")) {
                    String text = cmd.substring(7);
                    sendString(text.c_str());
                    delay(80);
                    continue;
                }

                if (cmd == "ENTER") {
                    runMacro("ENTER");
                    delay(80);
                    continue;
                }

                if (cmd.indexOf('+') != -1) {
                    runMacro(cmd);
                    delay(120);
                    continue;
                }

                runMacro(cmd);
                delay(80);
            }
        }
    }

    if (cfg.text.length() > 0) {
        send_text(cfg.text);
    }
}

// ----------------------------------------------------
//  Scan multi-touches sécurisé
// ----------------------------------------------------
void checkButtons() {

    for (int id = 1; id <= KEY_COUNT; id++) {

        int gpio = gpioMap[id];

        if (!isValidGPIO(gpio)) continue;

        int state = digitalRead(gpio);

        if (state == LOW && !keyPressed[id]) {

            delay(5); // anti-rebond simple
            if (digitalRead(gpio) != LOW) continue;

            LedEngine::notifyKeypress(120);

            KeyConfig cfg = WebServerManager::getKeyConfig(id);
            executeKeyAction(cfg);

            keyPressed[id] = true;
        }
        else if (state == HIGH) {
            keyPressed[id] = false;
        }
    }
}

// ----------------------------------------------------
//  SETUP
// ----------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("[BOOT] Init Macropad");

    // BLE
    bleKeyboard.begin();
    delay(200);

    // Web + chargement GPIO + application dynamique
    WebServerManager::begin();

    // LED
    recreateStrip();
    LedEngine::restoreState();

    onBleDisconnected();

    // ----------------------------------------------------
    //  🔥 Charger layout sauvegardé
    // ----------------------------------------------------
    Preferences prefs;
    prefs.begin("layout", false);
    String saved = prefs.getString("layout", "BE");

    if (saved == "FR") setLayout(LAYOUT_FR);
    else if (saved == "US") setLayout(LAYOUT_US);
    else setLayout(LAYOUT_BE);

    prefs.end();

    Serial.println("[BOOT] Ready");
}

// ----------------------------------------------------
//  LOOP
// ----------------------------------------------------
void loop() {
    WebServerManager::handle();
    checkButtons();

    LedEngine::update();

    static bool lastBleState = false;
    bool currentBleState = bleKeyboard.isConnected();

    if (currentBleState != lastBleState) {
        if (currentBleState) onBleConnected();
        else onBleDisconnected();
        lastBleState = currentBleState;
    }

    // ⚠️ Plus de fade doublon ici : LedEngine::setUserPower()
    // gère déjà le fade-in / fade-out proprement.

    delay(10);
}
