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
#include <FastLED.h>
#include "../globals.h"

namespace LedEngine {

    // Modes d’animation principaux
    enum LedMode : uint8_t {
        LED_MODE_OFF = 0,
        LED_MODE_STATIC,
        LED_MODE_BREATHING,
        LED_MODE_RAINBOW,
        LED_MODE_WAVE,
        LED_MODE_IDLE,

        // 🔥 Nouveaux modes multi‑couleurs
        LED_MODE_AURORA,
        LED_MODE_GRADIENT_FLOW,
        LED_MODE_PLASMA,
        LED_MODE_SEVE,

        // 🌸 Nouveaux modes pastel / chauds
        LED_MODE_PASTEL_FLOW,
        LED_MODE_WARM_GLOW,
        LED_MODE_SUNSET,
        LED_MODE_PASTEL_PALETTE,
        LED_MODE_COTTON_CANDY,
        LED_MODE_SOFT_AURORA,
        LED_MODE_DREAMWAVE
    };

    // ----------------------------------------------------
    // 🌟 API publique
    // ----------------------------------------------------

    // Couleur
    void setColor(const CRGB &color);
    void setColorHex(const String &hex);      // "#RRGGBB"

    // Luminosité
    void setBrightness(uint8_t brightness);   // 0–255

    // Mode d’animation
    void setMode(LedMode mode);
    void setModeByName(const String &name);   // "none", "breathing", "rainbow", ...

    // Vitesse d’animation (1–255)
    void setAnimationSpeed(uint8_t speed);

    // Effet lors d’un appui
    void setKeypressEffect(const String &name);

    // Flash / feedback lors d’une touche
    void notifyKeypress(uint16_t durationMs = 120);

    // ----------------------------------------------------
    // 🌿 Fade-in / Fade-out organique
    // ----------------------------------------------------
    void fadeTo(uint8_t targetBrightness, uint16_t durationMs);
    void fadeIn(uint16_t durationMs = 600);
    void fadeOut(uint16_t durationMs = 600);

    // ----------------------------------------------------
    // 💾 Sauvegarde / restauration du preset LED
    // ----------------------------------------------------
    void saveState();
    void restoreState();

    // Getter sur la dernière luminosité sauvegardée
    uint8_t getLastBrightness();

    // ----------------------------------------------------
    // 🔌 ON/OFF utilisateur (override)
    // ----------------------------------------------------
    // true = LEDs autorisées
    // false = LEDs forcées OFF (même si BLE dit ON)
    void setUserPower(bool on);
    bool getUserPower();

    // ----------------------------------------------------
    // Boucle principale
    // ----------------------------------------------------
    void update();
}
