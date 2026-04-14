#pragma once
#include <stdint.h>
#include "hid_types.h"

// Convertit un caractère Unicode en KeyInfo (HID + modificateurs)
KeyInfo getKeyFR(uint32_t unicode);
