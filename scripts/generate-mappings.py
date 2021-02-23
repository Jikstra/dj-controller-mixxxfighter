import os

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
            <file filename="MiniX1.midi.js" functionprefix="MiniX1"/>
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


def playButton(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>MiniX1.playButton</key>
                <description>Play button</description>
                <status>{hexStatus(C['MIDI_COMMAND_BUTTON'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_PLAY'])}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
    ''')

def syncButton(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>MiniX1.syncButton</key>
                <description>Beatsync button</description>
                <status>{hexStatus(C['MIDI_COMMAND_BUTTON'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_SYNC'])}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
    ''')

def monitorButton(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>pfl</key>
                <description>Monitor/CUE button</description>
                <status>{hexStatus(C['MIDI_COMMAND_BUTTON'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_MONITOR'])}</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def shiftButton():
    return(f'''
            <control>
                <group>[Master]</group>
                <key>MiniX1.shiftButton</key>
                <description>Shift button</description>
                <status>{hexStatus(C['MIDI_COMMAND_BUTTON'], str(0))}</status>
                <midino>{hexify(C['MIDI_CTRL_SHIFT'])}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
    ''')

def masterGainPoti():
    return(f'''
            <control>
                <group>[Master]</group>
                <key>gain</key>
                <description>Master gain potentiometer</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], str(0))}</status>
                <midino>{hexify(C['MIDI_CTRL_MASTER_GAIN'])}</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def masterHeadphoneGainPoti():
    return(f'''
            <control>
                <group>[Master]</group>
                <key>headGain</key>
                <description>Master headphone volume potentiometer</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], str(0))}</status>
                <midino>{hexify(C['MIDI_CTRL_MONITOR_VOLUME'])}</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def masterHeadphoneMixPoti():
    return(f'''
            <control>
                <group>[Master]</group>
                <key>headMix</key>
                <description>Master headphone volume potentiometer</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], str(0))}</status>
                <midino>{hexify(C['MIDI_CTRL_MONITOR_MIX'])}</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def bpmEncoder():
    return(f'''
            <control>
                <group>[Channel0]</group>
                <key>MiniX1.bpmEncoder</key>
                <description>BPM Rotary Encoder</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], str(0))}</status>
                <midino>{hexify(C['MIDI_CTRL_BPM'])}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
    ''')

def beatJumpEncoder(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>MiniX1.beatJumpEncoder</key>
                <description>BeatJump Rotary Encoder</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_BEAT_JUMP'])}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
    ''')

def beatJumpButton(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>MiniX1.beatJumpButton</key>
                <description>BeatJump button</description>
                <status>{hexStatus(C['MIDI_COMMAND_BUTTON'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_BEAT_JUMP'])}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
    ''')

def loopEncoder(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>MiniX1.loopEncoder</key>
                <description>loop Encoder</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_LOOP'])}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
    ''')

def loopButton(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>MiniX1.loopButton</key>
                <description>Shift button</description>
                <status>{hexStatus(C['MIDI_COMMAND_BUTTON'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_LOOP'])}</midino>
                <options>
                    <Script-Binding/>
                </options>
            </control>
    ''')

def volumePoti(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>volume</key>
                <description>Volume potentiometer</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_VOLUME'])}</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def gainPoti(ch):
    return(f'''
            <control>
                <group>[Channel{int(ch, 16)}]</group>
                <key>pregain</key>
                <description>Gain potentiometer</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], ch)}</status>
                <midino>{hexify(C['MIDI_CTRL_GAIN'])}</midino>
                <options>
                    <normal/>
                </options>
            </control>
    ''')

def generateEQPoti(parameter, midino):
    def eqRate(ch):
        return(f'''
                <control>
                    <group>[EqualizerRack1_[Channel{int(ch, 16)}]_Effect1]</group>
                    <key>parameter{parameter}</key>
                    <description>Equalizer for...</description>
                    <status>{hexStatus(C['MIDI_COMMAND_POTI'], ch)}</status>
                    <midino>{hexify(midino)}</midino>
                    <options>
                        <normal/>
                    </options>
                </control>
        ''')
    return eqRate

def generateFXPoti(parameter, midino):
    def fxRate(ch):
        return(f'''
            <control>
                <group>[EffectRack1_EffectUnit{int(ch, 16)}_Effect{parameter}]</group>
                <key>meta</key>
                <description>FX for...</description>
                <status>{hexStatus(C['MIDI_COMMAND_POTI'], ch)}</status>
                <midino>{hexify(midino)}</midino>
                <options>
                    <normal/>
                </options>
            </control>
            <control>
                <group>[EffectRack1_EffectUnit{int(ch, 16)}_Effect{parameter}]</group>
                <key>enabled</key>
                <description>FX enable/disable for...</description>
                <status>{hexStatus(C['MIDI_COMMAND_BUTTON'], ch)}</status>
                <midino>{hexify(midino)}</midino>
                <options>
                    <normal/>
                </options>
            </control>
        ''')
    return fxRate

def generateFXKill(parameter, midino):
    def fxKill(ch):
        return('''
        ''')
    return fxKill

def _allChannels(fnc):
    result = ''
    for ch in [C['MIDI_LEFT_DECK_CHANNEL'], C['MIDI_RIGHT_DECK_CHANNEL']]:
        result += fnc(ch)
    return result


if __name__ == '__main__':
    print(header())
    print(_allChannels(playButton))
    print(_allChannels(syncButton))
    print(_allChannels(monitorButton))
    print(_allChannels(volumePoti))
    print(_allChannels(gainPoti))

    print(shiftButton())
    print(masterGainPoti())
    print(masterHeadphoneGainPoti())
    print(masterHeadphoneMixPoti())
    print(bpmEncoder())
    
    print(_allChannels(beatJumpEncoder))
    print(_allChannels(beatJumpButton))

    print(_allChannels(loopEncoder))
    print(_allChannels(loopButton))
    
    print(_allChannels(generateEQPoti(1, C['MIDI_CTRL_EQ_LOW'])))
    print(_allChannels(generateEQPoti(2, C['MIDI_CTRL_EQ_MID'])))
    print(_allChannels(generateEQPoti(3, C['MIDI_CTRL_EQ_HIGH'])))

    print(_allChannels(generateFXPoti(1, C['MIDI_CTRL_FX1'])))
    print(_allChannels(generateFXPoti(2, C['MIDI_CTRL_FX2'])))
    print(_allChannels(generateFXPoti(3, C['MIDI_CTRL_FX3'])))
    print(footer())
