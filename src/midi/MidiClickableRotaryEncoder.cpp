#include "MidiClickableRotaryEncoder.h"

MidiClickableRotaryEncoder::MidiClickableRotaryEncoder(int pin_ccw, int pin_button, int pin_cw, int control, int channel) : ClickableRotaryEncoder(pin_ccw, pin_button, pin_cw), control(control), channel(channel) {}

void MidiClickableRotaryEncoder::onTurn(Direction direction) {
  DBG("MidiClickableRotaryEncoder [%i] Turned %s", pin_button, direction == Direction::CCW ? "CCW" : "CW");
  sendMIDI(0xB, channel, control, direction == Direction::CCW ? 0x01 : 0x7f);
}

void MidiClickableRotaryEncoder::onButtonPress() {
  DBG("MidiClickableRotaryEncoder [%i] Pressed!", pin_button);
  sendMIDI(0x9, channel, control, 0x7F);
}

void MidiClickableRotaryEncoder::onButtonRelease() {
  DBG("MidiClickableRotaryEncoder [%i] Released!", pin_button);
  sendMIDI(0x9, channel, control, 0x00);
}
