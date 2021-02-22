#include "MidiClickablePotentiometer.h"

MidiClickablePotentiometer::MidiClickablePotentiometer(int pin, int pin_button, int control, int channel) : ClickablePotentiometer(pin, pin_button), control(control), channel(channel) {}

void MidiClickablePotentiometer::onChange(int value) {
  int midiValue = map(value, 0, 1023, 0, 127);
  if (midiValue == lastMidiValue) return;
  lastMidiValue = midiValue;

  DBG("MidiClickablePotentiometer [%i] %i", pin, midiValue);
  sendMIDI(0xB, channel, control, midiValue);
}

void MidiClickablePotentiometer::onButtonPress() {
  DBG("MidiClickablePotentiometer [%i] Pressed!", pin_button);
  sendMIDI(0x9, channel, control, 0x7F);
}

void MidiClickablePotentiometer::onButtonRelease() {
  DBG("MidiClickablePotentiometer [%i] Released!", pin_button);
  sendMIDI(0x9, channel, control, 0x00);
}
