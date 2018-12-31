#ifndef EEPROM_H
#define EEPROM_H
#include <EEPROM.h>
// Current version of the EEPROM - increase when introducing breaking changes
#define EEPROM_VERSION 6

// EEPROM allocation for main
#define EEPROM_BASE 0
#define EEPROM_VERSION_COUNTER EEPROM_BASE
#define EEPROM_MODE (EEPROM_BASE + 1)
#define EEPROM_BRIGHTNESS (EEPROM_BASE + 2)
#define EEPROM_CYCLE (EEPROM_BASE + 3)

// EEPROM Allocation for BreathingEffect
#define EEPROM_BREATHING_COLOR_3BYTE (EEPROM_BASE + 4) // 3-BYTE COLOR
// === ADD OFFSET 3

#endif