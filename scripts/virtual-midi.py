#!/usr/bin/python

import rtmidi
import serial
import time
import traceback
import sys

SLEEP_INTERVAL = 0.3

NAME = 'MixxxFighter'

def printException(e):
    print("Error: ", e)
    print(traceback.format_exc())
    print(sys.exc_info()[0])
    time.sleep(SLEEP_INTERVAL)

def main(device):
    virtualMidiInput = rtmidi.MidiIn()
    virtualMidiInput.open_virtual_port(NAME)
    virtualMidiOutput = rtmidi.MidiOut()
    virtualMidiOutput.open_virtual_port(NAME)

    found_device = True
    while True:
        try:
            serialMidi = None
            try:
                serialMidi = serial.Serial(device, 115200, timeout=0, exclusive=True)
            except serial.SerialException as e:
                if found_device == True:
                    print("Could not find device ", device, ". Please connect it")
                    found_device = False

                continue
            except Exception as e:
                printException(e)
                serialMidi.close()

            print("Opened device", device)
            found_device = True

            try:
                while True:
                    buf = serialMidi.read(3)
                    len_buf = len(buf)
                    if len_buf == 3:
                        split = [buf[i] for i in range (0, len(buf))]
                        print("[MIDI <-]", hex(split[0]), hex(split[1]), hex(split[2]))
                        virtualMidiOutput.send_message(buf)
                    elif len_buf > 0:
                        print("[WARN] Buffer incomplete")

                    buf = virtualMidiInput.get_message()

                    if buf is not None:
                        print("[MIDI ->]", hex(buf[0][0]), hex(buf[0][1]), hex(buf[0][2]))
                        serialMidi.write(buf[0])
            except Exception as e:
                printException(e)
                serialMidi.close()
        except Exception as e:
            printException(e)

if __name__ == '__main__':
    device = '/dev/ttyUSB0'
    if len(sys.argv) == 2:
        device = sys.argv[1]
    main(device)
