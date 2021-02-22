#include "MidiButton.h"

MidiButton::MidiButton(int pin_button, int control, int channel) : Button(pin_button), control(control), channel(channel) {}

void MidiButton::onPress() {
  DBG("MidiButton [%i] Pressed!", pin_button);
  sendMIDI(MIDI_COMMAND_BUTTON, channel, control, MIDI_VALUE_BUTTON_PRESS);
}

void MidiButton::onRelease() {
  DBG("MidiButton [%i] Released!", pin_button);
  sendMIDI(MIDI_COMMAND_BUTTON, channel, control, MIDI_VALUE_BUTTON_RELEASE);
}
