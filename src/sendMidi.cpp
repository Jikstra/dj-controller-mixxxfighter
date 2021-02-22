#include "sendMidi.h"

void sendMIDI(int command, int channel, int note, int value) {
  int status = (command << 4) + channel;
  int data1 = note;
  int data2 = value;
  IFDEBUG(
    println("sendMIDI: 0x%02x %i %i", status, note, value);
    return
  );
  Serial.write(status);
  Serial.write(note);
  Serial.write(value);
}
