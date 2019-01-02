#ifndef EEPROM_H
#define EEPROM_H
#include <EEPROM.h>
// Current version of the EEPROM - increase when introducing breaking changes
#define EEPROM_VERSION 9

// EEPROM allocation for main
#define EEPROM_BASE 0
#define EEPROM_VERSION_COUNTER EEPROM_BASE
#define EEPROM_MODE (EEPROM_VERSION_COUNTER + 1)
#define EEPROM_BRIGHTNESS (EEPROM_MODE + 1)
#define EEPROM_CYCLE (EEPROM_BRIGHTNESS + 1)
#define EEPROM_PALETTE (EEPROM_CYCLE + 1)
// === ADD OFFSET 19 (1 + 3 * 6 for color palette)
#define EEPROM_PALETTE_END (EEPROM_PALETTE + 19)

// EEPROM Allocation for BreathingEffect
#define EEPROM_BREATHING_COLOR (EEPROM_PALETTE_END + 1)
#define EEPROM_BREATHING_FULLPALETTE (EEPROM_BREATHING_COLOR + 1)

#endif