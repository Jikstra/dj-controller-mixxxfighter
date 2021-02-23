
#include "LEDMatrix.h"



LEDMatrix::LEDMatrix(int* column_pins, int count_column_pins, int* row_pins, int count_row_pins) :
 column_pins(column_pins),
 count_column_pins(count_column_pins),
 row_pins(row_pins),
 count_row_pins(count_row_pins) {}


void LEDMatrix::setup() {
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

void LEDMatrix::setupRow(int row_pin) {
  pinMode(row_pin, OUTPUT);  
  digitalWrite(row_pin, LOW);
}

void LEDMatrix::setupColumn(int column_pin) {
  pinMode(column_pin, OUTPUT);  
  digitalWrite(column_pin, HIGH);
}

void LEDMatrix::selectRow(int row_index) {
  if (row_index > count_row_pins) return;
  if (selected_row_index != -1) unselectRow();
  
  selected_row_index = row_index;
  digitalWrite(row_pins[row_index], HIGH);
}
void LEDMatrix::unselectRow() {
  if (selected_row_index == -1) return;
  
  digitalWrite(row_pins[selected_row_index], LOW);
  selected_row_index = -1;
}

void LEDMatrix::turnOn(int column_index) {
  if (column_index > count_column_pins) return;
  if (turned_on_column_index != -1) turnOff();

  turned_on_column_index = column_index;
  digitalWrite(column_pins[column_index], LOW);
}

void LEDMatrix::turnOff() {
  if (turned_on_column_index == -1) return;

  digitalWrite(column_pins[turned_on_column_index], HIGH);
  turned_on_column_index = -1;
}

void LEDMatrix::process() {
}

