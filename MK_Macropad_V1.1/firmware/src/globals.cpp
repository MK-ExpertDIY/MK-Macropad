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




#include "globals.h"

// -----------------------------
//  Clavier BLE global (définition réelle)
// -----------------------------
BleKeyboard bleKeyboard("MK Macropad", "MK", 100);

// -----------------------------
//  Définition du ruban LED (NeoPixelBus)
// -----------------------------
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip = nullptr;

// -----------------------------
//  États des 9 touches
//  (index 1..9, index 0 inutilisé)
// -----------------------------
bool keyPressed[10] = { false };

// -----------------------------
//  Mapping GPIO (chargé par WebServerManager)
//  gpioMap[0] = GPIO LED
//  gpioMap[1..9] = GPIO touches
// -----------------------------
int gpioMap[10] = {
    -1,  // 0 = LED (désactivée par défaut)
    -1,  // 1
    -1,  // 2
    -1,  // 3
    -1,  // 4
    -1,  // 5
    -1,  // 6
    -1,  // 7
    -1,  // 8
    -1   // 9
};

// -----------------------------
//  Validation GPIO simple
// -----------------------------
bool isValidGPIO(int gpio) {
    return gpio >= 0 && gpio <= 39;
}

// -----------------------------
//  Recréation dynamique du strip LED
// -----------------------------
void recreateStrip() {
    if (strip != nullptr) {
        delete strip;
        strip = nullptr;
    }

    int pin = (gpioMap[0] != -1 ? gpioMap[0] : LED_PIN);

    strip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(NUM_LEDS, pin);
    strip->Begin();
    strip->Show(); // LEDs éteintes au départ
}

// -----------------------------
//  Application globale du mapping GPIO
// -----------------------------
void applyDynamicGPIO() {
    Serial.println("[GPIO] Reconfiguration globale");

    // Reset état touches
    for (int i = 1; i <= 9; i++) {
        keyPressed[i] = false;
    }

    // Touche 1..9
    for (int id = 1; id <= 9; id++) {
        int gpio = gpioMap[id];

        if (isValidGPIO(gpio)) {
            pinMode(gpio, INPUT_PULLUP);
            Serial.printf("[GPIO] Touche %d → GPIO %d\n", id, gpio);
        } else if (gpio != -1) {
            Serial.printf("[GPIO] Touche %d → GPIO %d invalide\n", id, gpio);
        }
    }

    // LED
    if (isValidGPIO(gpioMap[0])) {
        pinMode(gpioMap[0], OUTPUT);
        Serial.printf("[GPIO] LED → GPIO %d\n", gpioMap[0]);
    }

    // Recréation strip sur le bon GPIO
    recreateStrip();
}
