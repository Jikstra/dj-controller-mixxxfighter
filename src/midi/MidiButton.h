#pragma once

#include <Arduino.h>

#include "hardware/Button.h"
#include "log.h"
#include "sendMidi.h"
#include "constants.h"

class MidiButton : public Button {
  int channel;
  int control;

  public:
    MidiButton(int pin_button, int channel, int control);
    void onPress();
    void onRelease();
};
