#include "main.h"

const int CountLEDMatrixColumns = 4;
int LEDMatrixColumns[CountLEDMatrixColumns] = { A5, A4, A3, A2 };
const int CountLEDMatrixRows = 2;
int LEDMatrixRows[CountLEDMatrixRows] = { A1, A0 };
LEDMatrix* led_matrix = new LEDMatrix(LEDMatrixColumns, CountLEDMatrixColumns, LEDMatrixRows, CountLEDMatrixRows);

const int CountButtonMatrixColumns = 4;
int ButtonMatrixColumns[CountButtonMatrixColumns] = { 8, 9, 10, 11 };
const int CountButtonMatrixRows = 6;
int ButtonMatrixRows[CountButtonMatrixRows] = { 2, 3, 4, 5, 6, 7 };

ButtonMatrix* button_matrix = new ButtonMatrix(
  ButtonMatrixColumns,
  CountButtonMatrixColumns,
  ButtonMatrixRows,
  CountButtonMatrixRows
);

int button_midi_control_map[][6] = {
  {MIDI_CTRL_MODIFIER_PAGE_ONE, MIDI_CTRL_MODIFIER_PAGE_TWO, MIDI_CTRL_MODIFIER_PAGE_THREE, MIDI_CTRL_MODIFIER_PAGE_FOUR},
  {MIDI_CTRL_MODIFIER_FIVE, MIDI_CTRL_MODIFIER_SIX, MIDI_CTRL_MODIFIER_SEVEN, MIDI_CTRL_MODIFIER_EIGHT},
  {MIDI_CTRL_MODIFIER_ONE, MIDI_CTRL_MODIFIER_TWO, MIDI_CTRL_MODIFIER_THREE, MIDI_CTRL_MODIFIER_FOUR},
  {MIDI_CTRL_PLAY, MIDI_CTRL_SLOWER, MIDI_CTRL_FASTER, MIDI_CTRL_EMERGENCY_LOOP},
  {MIDI_CTRL_LEFT, MIDI_CTRL_UP, MIDI_CTRL_DOWN, MIDI_CTRL_RIGHT},
  {MIDI_CTRL_CH1, MIDI_CTRL_CH2, MIDI_CTRL_CH3, MIDI_CTRL_CH4}
};

int _led_matrix_tick = 0;

bool led_matrix_state[][4] = {
  {false, false, false, false},
  {false, false, false, false}
};

MidiButton* shiftButton = new MidiButton(13, MIDI_CTRL_SHIFT, MIDI_CHANNEL_ZERO);

void led_matrix_tick() {
  int row_index = _led_matrix_tick / 4;
  int column_index = _led_matrix_tick % 4;
  
  //DBG("_led_matrix_tick = %i %i:%i", _led_matrix_tick, row_index, column_index);
  
  bool led_state = led_matrix_state[row_index][column_index];
  led_matrix->turnOff();
  led_matrix->selectRow(row_index);
  if (led_state == true) {
    led_matrix->turnOn(column_index);
  }  
  _led_matrix_tick = _led_matrix_tick + 1;
  if (_led_matrix_tick > 7) _led_matrix_tick = 0;
}


void setup() { 
  Serial.begin(115200);
  led_matrix->setup();
  button_matrix->setup();
  shiftButton->setup();
}

int midi_note_to_row_index(int midi_note) {
  if (midi_note <= MIDI_CTRL_CH4) {
    return 0;
  } else if (midi_note >= MIDI_CTRL_MODIFIER_PAGE_ONE && midi_note <= MIDI_CTRL_MODIFIER_PAGE_FOUR) {
    return 1;
  }
  return -1;
}

int midi_note_to_column_index(int midi_note) {
  if (midi_note <= MIDI_CTRL_CH4) {
    return midi_note;
  } else if (midi_note >= MIDI_CTRL_MODIFIER_PAGE_ONE && midi_note <= MIDI_CTRL_MODIFIER_PAGE_FOUR) {
    return midi_note - MIDI_CTRL_MODIFIER_PAGE_ONE;
  }
  return -1;
}

void button_matrix_process_row(int row_index) {
  button_matrix->selectRow(row_index);
  for (int column_index = 0; column_index < CountButtonMatrixColumns; column_index++) {
    ButtonState button_state = button_matrix->buttonStateColumn(column_index);
    if (button_state == ButtonState::Unchanged) continue;
    DBG("%i:%i = %s", row_index, column_index, buttonStateToString(button_state));
    
    int midi_control = button_midi_control_map[row_index][column_index];

    sendMIDI(
      MIDI_COMMAND_BUTTON,
      MIDI_CHANNEL_ZERO,
      midi_control,
      button_state == ButtonState::Pressed ?
        MIDI_VALUE_BUTTON_PRESSED :
        MIDI_VALUE_BUTTON_RELEASE
    );
  }
  button_matrix->unselectRow();
}

void process_midi_read() {
  if (Serial.available() < 3) return;
  int first_byte = Serial.read();

  int note = Serial.read();
  int value = Serial.read();
  int cmd = first_byte >> 4;
  int channel = first_byte & 0x0F;

  DBG("MIDI IN: %i %i %i %i", cmd, channel, note, value);

  if (channel != 0) {
    DBG("INVALID MIDI: Channel must be zero.");
    return;
  }
  int row_index = midi_note_to_row_index(note);
  if (row_index == -1) {
    DBG("INVALID MIDI: Midi note is invalid (wrong row index)");
    return;
  }
  int column_index = midi_note_to_column_index(note);
  if (column_index == -1) {
    DBG("INVALID MIDI: Midi note is invalid (wrong column index)");
    return;
  }

  DBG("MIDI IN: Turning %s led %i:%i", (cmd == MIDI_COMMAND_LED_ON ? "on" : "off"), row_index, column_index);

  led_matrix_state[row_index][column_index] = cmd == MIDI_COMMAND_LED_ON ? true : false;
}

void loop() {
  process_midi_read();
  
  for (int row_index = 0; row_index < CountButtonMatrixRows; row_index++) {
    button_matrix_process_row(row_index);
    led_matrix_tick();
  }

  shiftButton->process();
}
