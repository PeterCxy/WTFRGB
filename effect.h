#ifndef EFFECT_H
#define EFFECT_H

class Effect {
 public:
  virtual void onUpdate(){};
  // On RESET, all variables should be set to DEFAULT value
  // NOT the value in EEPROM
  virtual void reset(){};
  // Load all values from EEPROM
  virtual void loadFromEEPROM(){};
  virtual void writeToEEPROM(){};
};

#endif