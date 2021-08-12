#!/usr/bin/python
import sys
import os
import inspect

PATH_THIS_SCRIPT = os.path.realpath(__file__)

PATH_CONSTANTS = os.path.normpath(os.path.join(PATH_THIS_SCRIPT, '..', '..', 'src', 'constants.h'))


# Parse constants from src/constants.h and put them in global C variable
def parseConstants():
    f = open(PATH_CONSTANTS, 'r')
    constants = {}
    for line in f:
        if line.find("=") == -1:
            continue
        line = line.strip().replace(';', '')
        splitted_line = line.split(' ')
        key = splitted_line[2]
        value = splitted_line[4].replace('"', '')
        constants[key] = value
    return constants
C = parseConstants()


def header():
    return(f'''<?xml version='1.0' encoding='utf-8'?>
<MixxxControllerPreset mixxxVersion="" schemaVersion="1">
    <info/>
    <controller id="{C['DEVICE_NAME']}">
        <scriptfiles>
            <file filename="MixxxFighter.midi.js" functionprefix="MixxxFighter"/>
        </scriptfiles>
        <controls>
    ''')

def footer():
    return('''
        </controls>
        <outputs/>
    </controller>
</MixxxControllerPreset>''')

def hexStatus(command, ch):
    return str(hex(int(command, 16) << 4 | int(ch, 16)))

def hexify(str):
    return hex(int(str, 16))

myFname = lambda: inspect.stack()[1][3]


def button(jsMethod, description, ctrl):
    print(f'''
            <control>
                <group>[Channel1]</group>
                <key>MixxxFighter.{jsMethod}</key>
                <description>{description}</description>
                <status>{hexStatus(C['MIDI_COMMAND_BUTTON'], C['MIDI_CHANNEL_ZERO'])}</status>
                <midino>{hexify(ctrl)}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
        
    ''')

if __name__ == '__main__':
    print(header())
    button("channelOneButton",   "Channel one button",   C['MIDI_CTRL_CH1'])
    button("channelTwoButton",   "Channel two button",   C['MIDI_CTRL_CH2'])
    button("channelThreeButton", "Channel three button", C['MIDI_CTRL_CH3'])
    button("channelFourButton",  "Channel four button",  C['MIDI_CTRL_CH4'])

    button("leftButton",  "Left button",  C['MIDI_CTRL_LEFT'])
    button("upButton",    "Up button",    C['MIDI_CTRL_UP'])
    button("downButton",  "Down button",  C['MIDI_CTRL_DOWN'])
    button("rightButton", "Right button", C['MIDI_CTRL_RIGHT'])

    button("nudgeBackButton",          "Nudge back while paused, make deck temporary slower while playing",               C['MIDI_CTRL_PLAY'])
    button("nudgeLittleBackButton",    "Nudge little back while paused, make deck temporary little slower while playing", C['MIDI_CTRL_SLOWER'])
    button("nudgeLittleForwardButton", "Nudge little forward while paused, make deck temporary faster while playing",     C['MIDI_CTRL_FASTER'])
    button("nudgeForwardButton",       "Nudge forward while paused, make deck faster while playing",                      C['MIDI_CTRL_EMERGENCY_LOOP'])

    button("shiftButton", "Shift button", C['MIDI_CTRL_SHIFT'])

    button("modifierOneButton",   "Modifier one button",   C['MIDI_CTRL_MODIFIER_ONE'])
    button("modifierTwoButton",   "Modifier two button",   C['MIDI_CTRL_MODIFIER_TWO'])
    button("modifierThreeButton", "Modifier three button", C['MIDI_CTRL_MODIFIER_THREE'])
    button("modifierFourButton",  "Modifier four button",  C['MIDI_CTRL_MODIFIER_FOUR'])
    button("modifierFiveButton",  "Modifier five button",  C['MIDI_CTRL_MODIFIER_FIVE'])
    button("modifierSixButton",   "Modifier six button",   C['MIDI_CTRL_MODIFIER_SIX'])
    button("modifierSevenButton", "Modifier seven button", C['MIDI_CTRL_MODIFIER_SEVEN'])
    button("modifierEightButton", "Modifier eight button", C['MIDI_CTRL_MODIFIER_EIGHT'])

    button("modifierPageOneButton", "Switch to modifier page one", C['MIDI_CTRL_MODIFIER_PAGE_ONE'])
    button("modifierPageTwoButton", "Switch to modifier page two", C['MIDI_CTRL_MODIFIER_PAGE_TWO'])
    button("modifierPageThreeButton", "Switch to modifier page three", C['MIDI_CTRL_MODIFIER_PAGE_THREE'])
    button("modifierPageFourButton", "Switch to modifier page four", C['MIDI_CTRL_MODIFIER_PAGE_FOUR'])

    print(footer())
