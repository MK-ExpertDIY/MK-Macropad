#include "layout_fr.h"

KeyInfo getKeyFR(uint32_t c) {
    switch(c) {

        // Lettres minuscules
        case U'a': return {20,false,false};
        case U'b': return {5,false,false};
        case U'c': return {6,false,false};
        case U'd': return {7,false,false};
        case U'e': return {8,false,false};
        case U'f': return {9,false,false};
        case U'g': return {10,false,false};
        case U'h': return {11,false,false};
        case U'i': return {12,false,false};
        case U'j': return {13,false,false};
        case U'k': return {14,false,false};
        case U'l': return {15,false,false};
        case U'm': return {16,false,false};   // ✔ corrigé
        case U'n': return {17,false,false};
        case U'o': return {18,false,false};
        case U'p': return {19,false,false};
        case U'q': return {4,false,false};
        case U'r': return {21,false,false};
        case U's': return {22,false,false};
        case U't': return {23,false,false};
        case U'u': return {24,false,false};
        case U'v': return {25,false,false};
        case U'w': return {29,false,false};
        case U'x': return {27,false,false};
        case U'y': return {28,false,false};
        case U'z': return {26,false,false};

        // Lettres majuscules
        case U'A': return {20,true,false};
        case U'B': return {5,true,false};
        case U'C': return {6,true,false};
        case U'D': return {7,true,false};
        case U'E': return {8,true,false};
        case U'F': return {9,true,false};
        case U'G': return {10,true,false};
        case U'H': return {11,true,false};
        case U'I': return {12,true,false};
        case U'J': return {13,true,false};
        case U'K': return {14,true,false};
        case U'L': return {15,true,false};
        case U'M': return {16,true,false};   // ✔ corrigé
        case U'N': return {17,true,false};
        case U'O': return {18,true,false};
        case U'P': return {19,true,false};
        case U'Q': return {4,true,false};
        case U'R': return {21,true,false};
        case U'S': return {22,true,false};
        case U'T': return {23,true,false};
        case U'U': return {24,true,false};
        case U'V': return {25,true,false};
        case U'W': return {29,true,false};
        case U'X': return {27,true,false};
        case U'Y': return {28,true,false};
        case U'Z': return {26,true,false};

        // Chiffres (FR = Shift obligatoire)
        case U'&': return {30,false,false};
        case U'é': return {31,false,false};
        case U'"': return {32,false,false};
        case U'\'': return {33,false,false};
        case U'(': return {34,false,false};
        case U'-': return {35,false,false};
        case U'è': return {36,false,false};
        case U'_': return {37,false,false};
        case U'ç': return {38,false,false};
        case U'à': return {39,false,false};

        // Symboles
        case U')': return {45,false,false};
        case U'=': return {46,false,false};
        case U'^': return {47,false,false};
        case U'$': return {48,false,false};
        case U'*': return {49,false,false};
        case U',': return {16,false,false};
        case U';': return {51,false,false};      // ; = semicolon
        case U':': return {51,true,false};       // ✔ corrigé (Shift obligatoire)
        case U'!': return {56,false,false};
        case U' ': return {44,false,false};

        // Shift
        case U'?': return {54,true,false};       // ✔ corrigé
        case U'.': return {51,true,false};       // ✔ corrigé
        case U'/': return {51,true,false};       // ✔ correct
        case U'+': return {56,true,false};

        // AltGr
        case U'@': return {31,false,true};
        case U'#': return {32,false,true};
        case U'{': return {33,false,true};
        case U'}': return {34,false,true};
        case U'[': return {47,false,true};
        case U']': return {48,false,true};
        case U'~': return {49,false,true};
        case U'\\': return {100,false,true};
        case U'|': return {100,true,true};

        default: return {0,false,false};
    }
}
