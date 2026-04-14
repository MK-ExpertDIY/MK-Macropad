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



#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "ble/BleKeyboard.h"
#include <NeoPixelBus.h>
#include <FastLED.h>   // CRGB, CHSV, rgb2hsv_approximate

// --- HID / Layouts ---
#include "hid/hid_types.h"   // KeyInfo
#include "hid/layout_be.h"
#include "hid/layout_fr.h"
#include "hid/layout_us.h"

// Nom du fichier de configuration clavier
#define KEYCONFIG_FILE "/keyconfig.txt"

// -----------------------------
//  LED STRIP
//  LED_PIN = valeur par défaut si gpioMap[0] == -1
// -----------------------------
#define LED_PIN 4
#define NUM_LEDS 18

// Strip NeoPixelBus (instancié dynamiquement)
extern NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip;

// Fonction pour recréer le strip après changement de GPIO
void recreateStrip();

// -----------------------------
//  Clavier BLE global
// -----------------------------
extern BleKeyboard bleKeyboard;

// -----------------------------
//  Multi‑touche + LED
//  gpioMap[0]   = GPIO LED (configurable via /api/gpio-map)
//  gpioMap[1..9] = GPIO touches
// -----------------------------
extern int gpioMap[10];

// -----------------------------
//  États des touches
//  keyPressed[1..9] = true/false
// -----------------------------
extern bool keyPressed[10];

// -----------------------------
//  Validation GPIO simple
// -----------------------------
bool isValidGPIO(int gpio);

// -----------------------------
//  Application globale du mapping GPIO
//  (pinMode, reset états, strip LED)
// -----------------------------
void applyDynamicGPIO();

// -----------------------------
//  Structure de configuration d'une touche
// -----------------------------
struct KeyConfig {
    String type;       // "sequence", "launch_app", etc.
    String text;       // texte libre
    String appPath;    // chemin complet
    String appName;    // nom auto
    String esp;        // id ESP32
    std::vector<String> sequence;
};

// -----------------------------
//  Layout clavier global
// -----------------------------
enum Layout {
    LAYOUT_BE,
    LAYOUT_FR,
    LAYOUT_US
};

extern Layout currentLayout;

// Pointeur vers la fonction de mapping active
extern KeyInfo (*getKey)(uint32_t c);

// Setter propre pour changer de layout
void setLayout(Layout l);
