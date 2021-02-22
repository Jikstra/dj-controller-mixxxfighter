
#include "Matrix.h"



Matrix::Matrix(int column_pins[], int row_pins[]) : column_pins(column_pins), row_pins(row_pins) {}


void Matrix::setup() {

  int size_column_pins = sizeof(column_pins)/sizeof(*column_pins);
  for (int column_pin_index = 0; column_pin_index < size_column_pins; column_pin_index++) {
    int column_pin = column_pins[column_pin_index];
    DBG("Column pin %i = %i", column_pin_index, column_pin);
  }
}


void Matrix::process() {
}

