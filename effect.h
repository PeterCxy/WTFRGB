#ifndef EFFECT_H
#define EFFECT_H

class Effect {
 public:
  virtual void onUpdate(){};
  // On RESET, all variables should be set to DEFAULT value
  // NOT the value in EEPROM
  // This may share some code with onParamUpdate()
  virtual void reset(){};
  // Called when parameters are updated (e.g. color palette has been changed)
  // Usually this updates the effects with the latest parameters
  // WITHOUT doing a full reset (i.e. keeping the current progress)
  virtual void onParamUpdate(){};
  // Load all values from EEPROM
  virtual void loadFromEEPROM(){};
  virtual void writeToEEPROM(){};
  // Serial command events
  virtual void handleCommand(char *cmdBuf, int len) {};
};

#endif