#!/usr/bin/python
import sys
import serial

def sendMidi(device, command, channel, note, value):
    serialMidi = serial.Serial(device, 115200, timeout=1, exclusive=False)
    first_byte = (command << 4) + channel
    print("[MIDI ->] {} {} {}".format(hex(first_byte), hex(note), hex(value)))  
    serialMidi.write([first_byte, note, value])


def help():
    print ("""
send-serial-midi: <device> <command> <channel> <note> <value>

Example: send-serial-midi /dev/ttyUSB0 0x9 0 127 1""")


def string_to_int(string):
    integer = None
    try:
        integer = int(string)
    except ValueError:
        pass
    try:
        integer = int(string, base=16)
    except ValueError:
        pass

    if integer is None:
        raise Exception("Cannot parse " + string)
    return integer

def main():
    show_help = len(sys.argv) != 6
    
    parsed_argv = []
    if show_help is False:
        for i in range(2, len(sys.argv)):
            parsed_argv.append(string_to_int(sys.argv[i]))
    print(parsed_argv)

    if show_help is True:
        help()
        return
    device = sys.argv[1]
    sendMidi(device, parsed_argv[0], parsed_argv[1], parsed_argv[2], parsed_argv[3])

if __name__ == "__main__":
    main()


