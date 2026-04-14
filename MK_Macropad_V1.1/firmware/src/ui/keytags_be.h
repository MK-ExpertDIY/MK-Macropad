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
#include <stdint.h>

struct KeyTag {
    const char* label;    // Texte affiché dans l'UI
    uint32_t unicode;     // Caractère envoyé au firmware
    bool isModifier;      // True si c'est Ctrl, Alt, Shift, Win, AltGr
};

// Liste des touches affichées dans l'UI pour le layout BE
static const KeyTag keytagsBE[] = {

    // Modificateurs
    {"Ctrl",   0, true},
    {"Shift",  0, true},
    {"Alt",    0, true},
    {"AltGr",  0, true},
    {"Win",    0, true},

    // Lettres
    {"A", 'a', false}, {"B", 'b', false}, {"C", 'c', false}, {"D", 'd', false},
    {"E", 'e', false}, {"F", 'f', false}, {"G", 'g', false}, {"H", 'h', false},
    {"I", 'i', false}, {"J", 'j', false}, {"K", 'k', false}, {"L", 'l', false},
    {"M", 'm', false}, {"N", 'n', false}, {"O", 'o', false}, {"P", 'p', false},
    {"Q", 'q', false}, {"R", 'r', false}, {"S", 's', false}, {"T", 't', false},
    {"U", 'u', false}, {"V", 'v', false}, {"W", 'w', false}, {"X", 'x', false},
    {"Y", 'y', false}, {"Z", 'z', false},

    // Chiffres
    {"0", '0', false}, {"1", '1', false}, {"2", '2', false}, {"3", '3', false},
    {"4", '4', false}, {"5", '5', false}, {"6", '6', false}, {"7", '7', false},
    {"8", '8', false}, {"9", '9', false},

    // Symboles BE
    {"&", '&', false}, {"é", 'é', false}, {"\"", '"', false}, {"'", '\'', false},
    {"(", '(', false}, {"§", '§', false}, {"è", 'è', false}, {"!", '!', false},
    {"ç", 'ç', false}, {"à", 'à', false}, {")", ')', false}, {"-", '-', false},
    {"^", '^', false}, {"$", '$', false}, {"µ", 'µ', false}, {";", ';', false},
    {":", ':', false}, {"=", '=', false}, {"<", '<', false}, {">", '>', false},
    {",", ',', false}, {".", '.', false},

    // AltGr
    {"@", '@', false}, {"#", '#', false}, {"{", '{', false}, {"}", '}', false},
    {"\\", '\\', false}, {"|", '|', false}, {"~", '~', false}, {"`", '`', false},

    // Touches système
    {"Enter",     '\n', false},
    {"Tab",       '\t', false},
    {"Backspace", 0x08, false},
    {"Esc",       0x1B, false},
    {"Space",     ' ',  false},
    {"Delete",    0x7F, false},
};
