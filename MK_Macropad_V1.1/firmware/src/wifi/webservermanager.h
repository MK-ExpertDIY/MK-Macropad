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
#include <vector>
#include <WiFi.h>          // <-- Corrige l'erreur WiFiClient non déclaré
#include "../globals.h"    // <-- Utilise la vraie définition de KeyConfig

class WebServerManager {
public:
    static void begin();
    static void handle();

    // Récupère la configuration d'une touche
    static KeyConfig getKeyConfig(uint8_t id);

    // Sauvegarde la configuration d'une touche
    static void saveKeyConfig(uint8_t id, const KeyConfig &cfg);

    // Envoi JSON (utilisé par l’API)
    static void sendJson(WiFiClient &client, const String &json);
};
