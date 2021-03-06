
#include "ButtonMatrix.h"



ButtonMatrix::ButtonMatrix(int* column_pins, int count_column_pins, int* row_pins, int count_row_pins) :
 column_pins(column_pins),
 count_column_pins(count_column_pins),
 row_pins(row_pins),
 count_row_pins(count_row_pins) {
   buttons = new Button* [count_row_pins];
   for (int row_pin_index = 0; row_pin_index < count_row_pins; row_pin_index++) {
     buttons[row_pin_index] = new Button [count_column_pins](-1);
   }
 }



void ButtonMatrix::setup() {
  for (int row_pin_index = 0; row_pin_index < count_row_pins; row_pin_index++) {
    int row_pin = row_pins[row_pin_index];
    DBG("row_pin %i = %i", row_pin_index, row_pin);
    setupRow(row_pin); 
  }
  for (int column_pin_index = 0; column_pin_index < count_column_pins; column_pin_index++) {
    int column_pin = column_pins[column_pin_index];
    DBG("Column pin %i = %i", column_pin_index, column_pin);
    setupColumn(column_pin);
  }
}

void ButtonMatrix::setupRow(int row_pin) {
  pinMode(row_pin, INPUT);  
}

void ButtonMatrix::setupColumn(int column_pin) {
  pinMode(column_pin, INPUT_PULLUP); 
}

void ButtonMatrix::selectRow(int row_index) {
  if (row_index > count_row_pins) return;
  if (selected_row_index != -1) unselectRow();

  selected_row_index = row_index;
  int row_pin = row_pins[row_index];
  pinMode(row_pin, OUTPUT);
  digitalWrite(row_pin, LOW);
}

void ButtonMatrix::unselectRow() {
  if (selected_row_index == -1) return;

  int row_pin = row_pins[selected_row_index];
  setupRow(row_pin);
  selected_row_index = -1;
}


ButtonState ButtonMatrix::buttonStateColumn(int column_index) {
  if (column_index > count_column_pins) {
    return;
  }
  int column_pin = column_pins[column_index];
  pinMode(column_pin, INPUT_PULLUP);
  int val = digitalRead(column_pin);
  pinMode(column_pin, INPUT);
  
  ButtonState button_state = buttons[selected_row_index][column_index]._buttonState(val);

  return button_state;  
}


void ButtonMatrix::process() {
}

