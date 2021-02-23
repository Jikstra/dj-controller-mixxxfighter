
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
  if (row_index > count_row_pins) {
    return;
  }
  digitalWrite(row_pins[row_index], HIGH);
}
void LEDMatrix::unselectRow(int row_index) {
  if (row_index > count_row_pins) {
    return;
  }
  digitalWrite(row_pins[row_index], LOW);
}

void LEDMatrix::turnOn(int column_index) {
  if (column_index > count_column_pins) {
    return;
  }
  digitalWrite(column_pins[column_index], LOW);
}

void LEDMatrix::turnOff(int column_index) {
  if (column_index > count_column_pins) {
    return;
  }
  digitalWrite(column_pins[column_index], HIGH);
}

void LEDMatrix::process() {
}

