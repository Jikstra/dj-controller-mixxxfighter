#pragma once

#include <Arduino.h>

#include "hardware/ClickablePotentiometer.h"
#include "log.h"
#include "sendMidi.h"

class MidiClickablePotentiometer : public ClickablePotentiometer {
  int channel;
  int control;
  int lastMidiValue = -1;

  public:
    MidiClickablePotentiometer(int pin, int pin_button, int channel, int control);
    void onChange(int value);
    void onButtonPress();
    void onButtonRelease();
};
