#pragma once

#include <Arduino.h>

#include "log.h"

void sendMIDI(int command, int channel, int note, int value);
