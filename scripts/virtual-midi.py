#!/usr/bin/python

import mido
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
    with mido.open_ioport(NAME, client_name=NAME, virtual=True) as virtualMidi:
        found_device = True
        while True:
            try:
                serialMidi = None
                try:
                    serialMidi = serial.Serial(device, 115200, timeout=1, exclusive=True)
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
                        if len(buf) == 3:
                            message = mido.parse(buf)
                            if message is None:
                                print("[ERR] Received non MIDI message")
                            if message is not None:
                                print("[MIDI <-]", message)
                                print("[MIDI <-]", message.hex())
                                virtualMidi.send(message)
                        buf = virtualMidi.poll()

                        if buf is not None:
                            print("[MIDI ->]", buf)
                            print("[MIDI ->]", buf.hex())
                            serialMidi.write(buf.bytes())
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
