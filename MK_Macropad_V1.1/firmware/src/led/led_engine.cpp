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



#include "led_engine.h"
#include "../globals.h"
#include <FastLED.h>  // CRGB, CHSV, rgb2hsv_approximate, random8...

namespace LedEngine {

    // ----------------------------------------------------
    // PROTOTYPES
    // ----------------------------------------------------
    void effectKeypressFlash();
    void effectKeypressRipple();
    void effectKeypressShockwave();
    void effectKeypressSparkle();
    void effectKeypressNebula();
    void effectKeypressFireburst();
    void effectKeypressElectricArc();
    void effectKeypressChromaticBloom();

    void effectStatic();
    void effectBreathing();
    void effectRainbow();
    void effectWave();
    void effectIdle();
    void effectAurora();
    void effectGradientFlow();
    void effectPlasma();
    void effectSeve();

    void effectPastelFlow();
    void effectWarmGlow();
    void effectSunset();
    void effectPastelPalette();
    void effectCottonCandy();
    void effectSoftAurora();
    void effectDreamwave();

    // ----------------------------------------------------
    // ÉTAT INTERNE
    // ----------------------------------------------------
    static LedMode currentMode = LED_MODE_OFF;
    static CRGB baseColor = CRGB::Black;
    static uint8_t currentBrightness = 255;

    static LedMode lastMode = LED_MODE_STATIC;
    static CRGB lastColor = CRGB::White;
    static uint8_t lastBrightness = 255;

    static uint16_t tBreath = 0;
    static uint16_t tWave = 0;
    static uint8_t rainbowOffset = 0;

    static uint8_t animationSpeed = 128;
    static uint16_t speedDivider = 1;

    static bool keypressActive = false;
    static uint32_t keypressEndMs = 0;
    static CRGB keypressColor = CRGB::White;
    static String keypressEffect = "none";

    static uint16_t ripplePos = 0;
    static bool rippleActive = false;

    static uint16_t shockwavePos = 0;
    static bool shockwaveActive = false;

    static uint16_t sparkleTimer = 0;
    static bool sparkleActive = false;

    // Buffer logique des LEDs
    static CRGB leds[NUM_LEDS];

    // ----------------------------------------------------
    // 🔌 ON/OFF UTILISATEUR
    // ----------------------------------------------------
    static bool userPowerOverride = true;

    // ----------------------------------------------------
    // 🌿 FADE
    // ----------------------------------------------------
    static bool fadeActive = false;
    static uint32_t fadeStart = 0;
    static uint16_t fadeDuration = 0;
    static uint8_t fadeFrom = 0;
    static uint8_t fadeTarget = 0;

    static float ease(float t) {
        return t * t * (3 - 2 * t);
    }

    void fadeTo(uint8_t target, uint16_t durationMs) {
        fadeActive = true;
        fadeStart = millis();
        fadeDuration = durationMs;
        fadeFrom = currentBrightness;
        fadeTarget = target;
    }

    void fadeIn(uint16_t durationMs) { fadeTo(255, durationMs); }
    void fadeOut(uint16_t durationMs) { fadeTo(0, durationMs); }

    // ----------------------------------------------------
    // 🔥 SET USER POWER
    // ----------------------------------------------------
    void setUserPower(bool on) {
        userPowerOverride = on;

        if (!on) {
            fadeTo(0, 800);
        } else {
            currentMode = lastMode;
            baseColor   = lastColor;
            fadeTo(lastBrightness, 800);
        }
    }

    bool getUserPower() { return userPowerOverride; }

    // ----------------------------------------------------
    // PROFILS DE VITESSE
    // ----------------------------------------------------
    struct AnimSpeedProfile {
        uint8_t minSpeed;
        uint8_t maxSpeed;
    };

    static const AnimSpeedProfile speedProfiles[] = {
        { 1, 1 },   // OFF
        { 1, 1 },   // STATIC
        { 5, 80 },  // BREATHING
        { 1, 10 },  // RAINBOW
        { 2, 20 },  // WAVE
        { 10,120 }, // IDLE
        { 5, 40 },  // AURORA
        { 1, 8 },   // GRADIENT_FLOW
        { 2, 15 },  // PLASMA
        { 5, 50 },  // SEVE
        { 5, 40 },  // PASTEL_FLOW
        { 10,80 },  // WARM_GLOW
        { 5, 50 },  // SUNSET
        { 1, 10 },  // PASTEL_PALETTE
        { 5, 40 },  // COTTON_CANDY
        { 5, 40 },  // SOFT_AURORA
        { 5, 40 }   // DREAMWAVE
    };

    // ----------------------------------------------------
    // SAUVEGARDE
    // ----------------------------------------------------
    void saveState() {
        lastMode = currentMode;
        lastColor = baseColor;
        lastBrightness = currentBrightness;
    }

    void restoreState() {
        currentMode = lastMode;
        baseColor = lastColor;
        // ❌ On ne restaure plus la luminosité ici
        // currentBrightness = lastBrightness;
    }

    uint8_t getLastBrightness() {
        return lastBrightness;
    }

    // ----------------------------------------------------
    // HELPERS NEO PIXEL BUS
    // ----------------------------------------------------
    inline void pushToStrip() {
        if (!strip) return;
        for (int i = 0; i < NUM_LEDS; i++) {
            CRGB c = leds[i];
            uint8_t r = (c.r * currentBrightness) / 255;
            uint8_t g = (c.g * currentBrightness) / 255;
            uint8_t b = (c.b * currentBrightness) / 255;
            strip->SetPixelColor(i, RgbColor(r, g, b));
        }
        strip->Show();
    }

    void applyStaticColor(const CRGB &c) {
        for (int i = 0; i < NUM_LEDS; i++) leds[i] = c;
        pushToStrip();
    }

    CRGB hexToColor(const String &hex) {
        if (!hex.startsWith("#") || hex.length() != 7) return CRGB::Black;
        long rgb = strtol(hex.substring(1).c_str(), nullptr, 16);
        return CRGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
    }

    // ----------------------------------------------------
    // API PUBLIQUE
    // ----------------------------------------------------
    void setColor(const CRGB &color) {
        baseColor = color;
        saveState();
    }

    void setColorHex(const String &hex) {
        baseColor = hexToColor(hex);
        saveState();
    }

    void setBrightness(uint8_t brightness) {
        currentBrightness = brightness;
        saveState();
    }

    void setAnimationSpeed(uint8_t speed) {
        animationSpeed = speed;

        uint8_t modeIndex = static_cast<uint8_t>(currentMode);
        if (modeIndex >= (sizeof(speedProfiles) / sizeof(speedProfiles[0]))) modeIndex = 0;

        const AnimSpeedProfile &p = speedProfiles[modeIndex];
        uint8_t mapped = map(speed, 1, 255, p.minSpeed, p.maxSpeed);
        if (mapped < 1) mapped = 1;
        speedDivider = mapped;
    }

    void setKeypressEffect(const String &name) {
        keypressEffect = name;
    }

    void setMode(LedMode mode) {
        currentMode = mode;
        saveState();
    }

    void setModeByName(const String &name) {
        String n = name; n.toLowerCase();

        if (n == "none" || n == "off" || n == "") setMode(LED_MODE_STATIC);
        else if (n == "breathing") setMode(LED_MODE_BREATHING);
        else if (n == "rainbow") setMode(LED_MODE_RAINBOW);
        else if (n == "wave") setMode(LED_MODE_WAVE);
        else if (n == "idle") setMode(LED_MODE_IDLE);
        else if (n == "aurora") setMode(LED_MODE_AURORA);
        else if (n == "gradient_flow") setMode(LED_MODE_GRADIENT_FLOW);
        else if (n == "plasma") setMode(LED_MODE_PLASMA);
        else if (n == "seve") setMode(LED_MODE_SEVE);

        else if (n == "pastel_flow") setMode(LED_MODE_PASTEL_FLOW);
        else if (n == "warm_glow") setMode(LED_MODE_WARM_GLOW);
        else if (n == "sunset") setMode(LED_MODE_SUNSET);
        else if (n == "pastel_palette") setMode(LED_MODE_PASTEL_PALETTE);
        else if (n == "cotton_candy") setMode(LED_MODE_COTTON_CANDY);
        else if (n == "soft_aurora") setMode(LED_MODE_SOFT_AURORA);
        else if (n == "dreamwave") setMode(LED_MODE_DREAMWAVE);

        else setMode(LED_MODE_STATIC);
    }

    // ----------------------------------------------------
    // KEYPRESS
    // ----------------------------------------------------
    void notifyKeypress(uint16_t durationMs) {
        if (keypressEffect == "none") return;

        keypressActive = true;
        keypressEndMs = millis() + durationMs;

        rippleActive = shockwaveActive = sparkleActive = false;

        if (keypressEffect == "flash_white") keypressColor = CRGB::White;
        else if (keypressEffect == "flash_rainbow") keypressColor = CHSV(random8(), 255, 255);
        else if (keypressEffect == "pulse") keypressColor = baseColor;
        else if (keypressEffect == "spark") { sparkleActive = true; sparkleTimer = 0; }
        else if (keypressEffect == "ripple") { rippleActive = true; ripplePos = 0; keypressColor = baseColor; }
        else if (keypressEffect == "shockwave") { shockwaveActive = true; shockwavePos = 0; keypressColor = baseColor; }
        else if (keypressEffect == "aurora_flash") keypressColor = CRGB(0, 180, 255);
        else if (keypressEffect == "seve_flash") keypressColor = CRGB(80, 200, 40);
        else if (keypressEffect == "nebula_flash") keypressColor = CRGB(80, 40, 120);
        else if (keypressEffect == "fireburst") keypressColor = CRGB(255, 120, 20);
        else if (keypressEffect == "electric_arc") keypressColor = CRGB(80, 160, 255);
        else if (keypressEffect == "chromatic_bloom") keypressColor = baseColor;
    }

    // ----------------------------------------------------
    // EFFETS KEYPRESS
    // ----------------------------------------------------
    void effectKeypressFlash() {
        for (int i = 0; i < NUM_LEDS; i++) leds[i] = keypressColor;
    }

    void effectKeypressRipple() {
        ripplePos += speedDivider;

        for (int i = 0; i < NUM_LEDS; i++) {
            int dist = abs(i - NUM_LEDS / 2);
            int wave = ripplePos - dist * 4;

            if (wave > 0 && wave < 40) {
                uint8_t intensity = map(wave, 0, 40, 255, 0);
                CRGB c = keypressColor;
                c.nscale8(intensity);
                leds[i] = c;
            } else {
                leds[i].fadeToBlackBy(40);
            }
        }

        if (ripplePos > NUM_LEDS * 6) rippleActive = false;
    }

    void effectKeypressShockwave() {
        shockwavePos += speedDivider;

        for (int i = 0; i < NUM_LEDS; i++) {
            int dist = abs(i - NUM_LEDS / 2);
            if (dist == shockwavePos / 4) leds[i] = keypressColor;
            else leds[i].fadeToBlackBy(40);
        }

        if (shockwavePos > NUM_LEDS * 8) shockwaveActive = false;
    }

    void effectKeypressSparkle() {
        sparkleTimer += speedDivider;

        for (int i = 0; i < NUM_LEDS; i++) {
            if (random8() < 20) leds[i] = CHSV(random8(), 255, 255);
            else leds[i].fadeToBlackBy(40);
        }

        if (sparkleTimer > 200) sparkleActive = false;
    }

    void effectKeypressNebula() {
        float t = (millis() / (float)speedDivider) * 0.0015f;
        for (int i = 0; i < NUM_LEDS; i++) {
            float n = sin(i * 0.25f + t) + sin(i * 0.11f - t * 0.7f);
            uint8_t hue = (uint8_t)((n + 2.0f) * 64);
            leds[i] = CHSV(hue, 180, 180);
        }
    }

    void effectKeypressFireburst() {
        int center = NUM_LEDS / 2;
        for (int i = 0; i < NUM_LEDS; i++) {
            int dist = abs(i - center);
            uint8_t intensity = dist < 10 ? map(dist, 0, 10, 255, 0) : 0;
            CRGB c = CRGB(255, 80, 10);
            c.nscale8(intensity);
            leds[i] = c;
        }
    }

    void effectKeypressElectricArc() {
        for (int i = 0; i < NUM_LEDS; i++) {
            if (random8() < 40) leds[i] = CRGB(80, 160, 255);
            else leds[i].fadeToBlackBy(60);
        }
    }

    void effectKeypressChromaticBloom() {
        int center = NUM_LEDS / 2;
        for (int i = 0; i < NUM_LEDS; i++) {
            int dist = abs(i - center);
            uint8_t intensity = dist < 12 ? map(dist, 0, 12, 255, 0) : 0;
            uint8_t hue = map(i, 0, NUM_LEDS - 1, 0, 255);
            CRGB c = CHSV(hue, 200, 255);
            c.nscale8(intensity);
            leds[i] = c;
        }
    }

    // ----------------------------------------------------
    // EFFETS PRINCIPAUX
    // ----------------------------------------------------
    void effectStatic() {
        for (int i = 0; i < NUM_LEDS; i++)
            leds[i] = baseColor;
    }

    void effectBreathing() {
        tBreath++;
        CHSV pivot = rgb2hsv_approximate(baseColor);

        float phase = (sin((tBreath / (float)speedDivider) * 0.0008f) + 1.0f) * 0.5f;
        uint8_t val = pivot.v * phase;

        for (int i = 0; i < NUM_LEDS; i++)
            leds[i] = CHSV(pivot.h, pivot.s, val);
    }

    void effectRainbow() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        rainbowOffset += speedDivider;

        for (int i = 0; i < NUM_LEDS; i++) {
            uint8_t hue = rainbowOffset + i * 3;
            hue = hue * 0.4 + pivot.h * 0.6;
            leds[i] = CHSV(hue, pivot.s, pivot.v);
        }
    }

    void effectWave() {
        tWave += speedDivider;
        CHSV pivot = rgb2hsv_approximate(baseColor);

        const uint8_t width = 6;

        for (int i = 0; i < NUM_LEDS; i++) {
            int pos = (i + (tWave / 20)) % NUM_LEDS;
            int dist = abs((NUM_LEDS / 2) - pos);
            uint8_t intensity = dist < width ? map(width - dist, 0, width, 0, pivot.v) : 0;

            uint8_t hue = pivot.h + sin(i * 0.1f + tWave * 0.01f) * 20;
            leds[i] = CHSV(hue, pivot.s, intensity);
        }
    }

    void effectIdle() {
        tBreath++;
        CHSV pivot = rgb2hsv_approximate(baseColor);

        float phase = (sin((tBreath / (float)speedDivider) * 0.0004f) + 1.0f) * 0.5f;
        uint8_t val = pivot.v * (0.3 + phase * 0.7);

        for (int i = 0; i < NUM_LEDS; i++)
            leds[i] = CHSV(pivot.h, pivot.s, val);
    }

    void effectAurora() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = (millis() / (float)speedDivider) * 0.001f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float wave = sin(i * 0.25f + t);

            uint8_t hue = pivot.h + wave * 25;
            uint8_t sat = pivot.s * 0.7;
            uint8_t val = pivot.v * (0.6 + wave * 0.4);

            leds[i] = CHSV(hue, sat, val);
        }
    }

        void effectGradientFlow() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        uint32_t t = millis() / speedDivider;

        for (int i = 0; i < NUM_LEDS; i++) {
            uint8_t hue = pivot.h + ((i * 4 + t / 4) % 40) - 20;
            leds[i] = CHSV(hue, pivot.s, pivot.v);
        }
    }

    void effectPlasma() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = (millis() / (float)speedDivider) * 0.002f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float v = sin(i * 0.3f + t);

            uint8_t hue = pivot.h + v * 40;
            uint8_t val = pivot.v * (0.6 + v * 0.4);

            leds[i] = CHSV(hue, pivot.s, val);
        }
    }

    void effectSeve() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = (millis() / (float)speedDivider) * 0.001f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float wave = sin(i * 0.15f + t);

            uint8_t hue = pivot.h + wave * 30;
            uint8_t sat = pivot.s * 0.8;
            uint8_t val = pivot.v * (0.5 + wave * 0.5);

            leds[i] = CHSV(hue, sat, val);
        }
    }

    void effectPastelFlow() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = millis() * 0.0005f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float wave = sin(i * 0.15f + t) * 0.5f + 0.5f;

            uint8_t hue = pivot.h + (wave * 40 - 20);
            uint8_t sat = pivot.s * 0.4;
            uint8_t val = pivot.v * (0.7 + wave * 0.3);

            leds[i] = CHSV(hue, sat, val);
        }
    }

    void effectWarmGlow() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = millis() * 0.001f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float flicker = sin(i * 0.2f + t) * 0.3f + 0.7f;

            uint8_t hue = pivot.h + random8(-10, 10);
            uint8_t val = pivot.v * flicker;

            leds[i] = CHSV(hue, pivot.s, val);
        }
    }

    void effectSunset() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = millis() * 0.0003f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float wave = sin(i * 0.1f + t) * 0.5f + 0.5f;

            uint8_t hue = pivot.h + wave * 30;
            uint8_t sat = pivot.s * 0.8;
            uint8_t val = pivot.v * (0.6 + wave * 0.4);

            leds[i] = CHSV(hue, sat, val);
        }
    }

    void effectPastelPalette() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        uint8_t index = millis() / 20;

        for (int i = 0; i < NUM_LEDS; i++) {
            uint8_t hue = pivot.h + sin((index + i * 4) * 0.05f) * 30;
            uint8_t sat = pivot.s * 0.4;
            uint8_t val = pivot.v;

            leds[i] = CHSV(hue, sat, val);
        }
    }

    void effectCottonCandy() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = millis() * 0.0006f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float w = sin(i * 0.18f + t) * 0.5f + 0.5f;

            uint8_t hue = pivot.h + (w < 0.5 ? -30 : 30);
            uint8_t sat = pivot.s * 0.4;
            uint8_t val = pivot.v;

            leds[i] = CHSV(hue, sat, val);
        }
    }

    void effectSoftAurora() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = millis() * 0.001f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float wave = sin(i * 0.25f + t);

            uint8_t hue = pivot.h + wave * 25;
            uint8_t sat = pivot.s * 0.5;
            uint8_t val = pivot.v;

            leds[i] = CHSV(hue, sat, val);
        }
    }

    void effectDreamwave() {
        CHSV pivot = rgb2hsv_approximate(baseColor);
        float t = millis() * 0.0004f;

        for (int i = 0; i < NUM_LEDS; i++) {
            float w = sin(i * 0.12f + t) * 0.5f + 0.5f;

            uint8_t hue = pivot.h + w * 40;
            uint8_t sat = pivot.s * 0.5;
            uint8_t val = pivot.v * (0.7 + w * 0.3);

            leds[i] = CHSV(hue, sat, val);
        }
    }

    // ----------------------------------------------------
    // Scheduler de frame
    // ----------------------------------------------------
    static uint32_t lastFrameMs = 0;
    static const uint16_t FRAME_INTERVAL_MS = 16; // ~60 FPS

    // ----------------------------------------------------
    // UPDATE PRINCIPAL
    // ----------------------------------------------------
    void update() {
        uint32_t now = millis();

        if (lastFrameMs == 0) lastFrameMs = now;
        uint32_t elapsedFrame = now - lastFrameMs;
        if (elapsedFrame < FRAME_INTERVAL_MS) return;
        lastFrameMs = now;

        if (!userPowerOverride && !fadeActive) {
            applyStaticColor(CRGB::Black);
            return;
        }

        if (fadeActive) {
            uint32_t elapsed = now - fadeStart;

            if (elapsed >= fadeDuration) {
                fadeActive = false;
                currentBrightness = fadeTarget;
            } else {
                float t = (float)elapsed / fadeDuration;
                float e = ease(t);
                uint8_t b = fadeFrom + (fadeTarget - fadeFrom) * e;
                currentBrightness = b;
            }
        }

        if (!fadeActive && currentBrightness == 0 && currentMode != LED_MODE_OFF)
            currentMode = LED_MODE_OFF;

        if (keypressActive) {
            if (now < keypressEndMs) {

                if (rippleActive)          effectKeypressRipple();
                else if (shockwaveActive)  effectKeypressShockwave();
                else if (sparkleActive)    effectKeypressSparkle();
                else if (keypressEffect == "nebula_flash")        effectKeypressNebula();
                else if (keypressEffect == "fireburst")           effectKeypressFireburst();
                else if (keypressEffect == "electric_arc")        effectKeypressElectricArc();
                else if (keypressEffect == "chromatic_bloom")     effectKeypressChromaticBloom();
                else                                              effectKeypressFlash();

                pushToStrip();
                return;
            }
            keypressActive = false;
        }

        switch (currentMode) {
            case LED_MODE_OFF:
                for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Black;
                break;

            case LED_MODE_STATIC:        effectStatic(); break;
            case LED_MODE_BREATHING:     effectBreathing(); break;
            case LED_MODE_RAINBOW:       effectRainbow(); break;
            case LED_MODE_WAVE:          effectWave(); break;
            case LED_MODE_IDLE:          effectIdle(); break;
            case LED_MODE_AURORA:        effectAurora(); break;
            case LED_MODE_GRADIENT_FLOW: effectGradientFlow(); break;
            case LED_MODE_PLASMA:        effectPlasma(); break;
            case LED_MODE_SEVE:          effectSeve(); break;

            case LED_MODE_PASTEL_FLOW:    effectPastelFlow(); break;
            case LED_MODE_WARM_GLOW:      effectWarmGlow(); break;
            case LED_MODE_SUNSET:         effectSunset(); break;
            case LED_MODE_PASTEL_PALETTE: effectPastelPalette(); break;
            case LED_MODE_COTTON_CANDY:   effectCottonCandy(); break;
            case LED_MODE_SOFT_AURORA:    effectSoftAurora(); break;
            case LED_MODE_DREAMWAVE:      effectDreamwave(); break;

            default:
                effectStatic();
                break;
        }

        pushToStrip();
    }
}
