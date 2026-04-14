#pragma once
#include <stdint.h>
#include "hid_types.h"

// Fonction publique : convertit un caractère Unicode en KeyInfo (HID + modificateurs)
KeyInfo getKeyBE(uint32_t unicode);
