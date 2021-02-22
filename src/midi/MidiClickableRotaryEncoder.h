#pragma once

#include <Arduino.h>

#include "hardware/ClickableRotaryEncoder.h"
#include "log.h"
#include "sendMidi.h"

class MidiClickableRotaryEncoder : public ClickableRotaryEncoder {
  int channel;
  int control;

  public:
    MidiClickableRotaryEncoder(int pin_ccw, int pin_button, int pin_cw, int channel, int control);
    void onTurn(Direction direction);
    void onButtonPress();
    void onButtonRelease();
};
