#include "main.h"


const int CountLEDMatrixColumns = 4;
int LEDMatrixColumns[CountLEDMatrixColumns] = { A2, A3, A4, A5};
const int CountLEDMatrixRows = 2;
int LEDMatrixRows[CountLEDMatrixRows] = { A0, A1 };
LEDMatrix* led_matrix = new LEDMatrix(LEDMatrixColumns, CountLEDMatrixColumns, LEDMatrixRows, CountLEDMatrixRows);

const int CountButtonMatrixColumns = 6;
int ButtonMatrixColumns[CountButtonMatrixColumns] = { 2, 3, 4, 5, 6, 7 };
const int CountButtonMatrixRows = 4;
int ButtonMatrixRows[CountButtonMatrixRows] = { 8, 9, 10, 11 };

ButtonMatrix* button_matrix = new ButtonMatrix(
  ButtonMatrixColumns,
  CountButtonMatrixColumns,
  ButtonMatrixRows,
  CountButtonMatrixRows
);

int button_midi_control_map[][6] = {
  {MIDI_CTRL_CH1, MIDI_CTRL_CH2, MIDI_CTRL_CH3, MIDI_CTRL_CH4},
  {MIDI_CTRL_LEFT, MIDI_CTRL_UP, MIDI_CTRL_DOWN, MIDI_CTRL_RIGHT},
  {MIDI_CTRL_PLAY, MIDI_CTRL_SLOWER, MIDI_CTRL_FASTER, MIDI_CTRL_EMERGENCY_LOOP},
  {MIDI_CTRL_MODIFIER_ONE, MIDI_CTRL_MODIFIER_TWO, MIDI_CTRL_MODIFIER_THREE, MIDI_CTRL_MODIFIER_FOUR},
  {MIDI_CTRL_MODIFIER_FIVE, MIDI_CTRL_MODIFIER_SIX, MIDI_CTRL_MODIFIER_SEVEN, MIDI_CTRL_MODIFIER_EIGHT},
  {MIDI_CTRL_MODIFIER_PAGE_ONE, MIDI_CTRL_MODIFIER_PAGE_TWO, MIDI_CTRL_MODIFIER_PAGE_THREE, MIDI_CTRL_MODIFIER_PAGE_FOUR}
};

int _led_matrix_tick = 0;

bool led_matrix_state[][4] = {
  {true, true, true, true},
  {true, true, true, true}
};

void led_matrix_tick() {
  int row_index = _led_matrix_tick / 4;
  int column_index = _led_matrix_tick % 4;
  
  DBG("_led_matrix_tick = %i %i:%i", _led_matrix_tick, row_index, column_index);
  
  bool led_state = led_matrix_state[row_index][column_index];
  if (led_state == true) {
    led_matrix->selectRow(row_index);
    led_matrix->turnOn(column_index);
  }
  
  _led_matrix_tick = _led_matrix_tick + 1;
  if (_led_matrix_tick > 7) _led_matrix_tick = 0;
}


void setup() { 
  Serial.begin(115200);
  led_matrix->setup();
  button_matrix->setup();
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

void loop() {
  /*for (int row_index = 0; row_index < CountLEDMatrixRows; row_index++) {
  }*/
  
  for (int row_index = 0; row_index < CountButtonMatrixRows; row_index++) {
    button_matrix_process_row(row_index);
    led_matrix_tick();
  }
}
