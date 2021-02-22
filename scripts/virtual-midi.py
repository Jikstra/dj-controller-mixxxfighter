import mido
import serial
import time
import traceback
import sys

SLEEP_INTERVAL = 0.3

def printException(e):
    print("Error: ", e)
    print(traceback.format_exc())
    print(sys.exc_info()[0])
    time.sleep(SLEEP_INTERVAL)

def main():
    device = '/dev/ttyACM0'
    with mido.open_output('MiniX1', client_name='MiniX1', virtual=True) as output:
        found_device = True
        while True:
            try:
                connection = None
                try:
                    connection = serial.Serial(device, 115200, timeout=1, exclusive=True)
                except serial.SerialException as e:
                    if found_device == True:
                        print("Could not find device ", device, ". Please connect it")
                        found_device = False

                    continue
                except Exception as e:
                    printException(e)
                    connection.close()

                print("Opened device", device)
                found_device = True

                try:
                    while True:
                        buf = connection.read(3)
                        if len(buf) != 3:
                            continue
                        message = mido.parse(buf)
                        print("[MIDI]", message)
                        print("[MIDI]", message.hex())
                        output.send(message)
                except Exception as e:
                    printException(e)
                    connection.close()
            except Exception as e:
                printException(e)


if __name__ == '__main__':
    main()
