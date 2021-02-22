#pragma once

const int DEVICE_NAME = "MiniX1";

const int MIDI_ALL_CHANNEL = 0x0;
const int MIDI_LEFT_DECK_CHANNEL = 0x1;
const int MIDI_RIGHT_DECK_CHANNEL = 0x2;

const int MIDI_CTRL_VOLUME = 0x1;
const int MIDI_CTRL_GAIN = 0x2;

const int MIDI_CTRL_EQ_LOW = 0xa;
const int MIDI_CTRL_EQ_MID = 0xb;
const int MIDI_CTRL_EQ_HIGH = 0xc;

const int MIDI_CTRL_FX1 = 0x14;
const int MIDI_CTRL_FX2 = 0x15;
const int MIDI_CTRL_FX3 = 0x16;

const int MIDI_CTRL_PLAY = 0x1e;
const int MIDI_CTRL_SYNC = 0x1f;
const int MIDI_CTRL_MONITOR = 0x20;
const int MIDI_CTRL_SHIFT = 0x27;
const int MIDI_CTRL_MASTER_GAIN = 0x50;
const int MIDI_CTRL_MONITOR_MIX = 0x51;
const int MIDI_CTRL_MONITOR_VOLUME = 0x52;
const int MIDI_CTRL_BPM = 0x53;


const int MIDI_CTRL_BEAT_JUMP = 0x28;
const int MIDI_CTRL_LOOP = 0x29;

const int MIDI_COMMAND_BUTTON = 0x9;
const int MIDI_COMMAND_POTI = 0xB;

const int MIDI_VALUE_BUTTON_RELEASE = 0x00;
const int MIDI_VALUE_BUTTON_PRESS = 0x7F; 
