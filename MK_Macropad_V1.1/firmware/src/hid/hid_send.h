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
#include "hid_types.h"     // KeyInfo
#include "layout_be.h"     // getKeyBE()
#include "ble/BleKeyboard.h"
#include "globals.h"       // bleKeyboard

// Envoi d’un caractère Unicode → HID
void sendChar(uint32_t c);

// Envoi d’une chaîne de texte
void sendString(const char* text);

// Exécution d’une macro (ex: "CTRL+C", "ENTER")
void runMacro(const String& macro);
