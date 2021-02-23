#pragma once

#include "log.h"
#include "BaseComponent.h"

class ButtonMatrix : public BaseComponent {
  public:
    int* column_pins;
    int count_column_pins;
    int* row_pins;
    int count_row_pins;

    ButtonMatrix(int* column_pins, int count_column_pins, int* row_pins, int count_row_pins);
    void setup();
    void setupRow(int row_pin);
    void setupColumn(int column_pin);
    void selectRow(int row_pin);
    void unselectRow(int row_pin);
    int digitalReadColumn(int column_pin);
    void process();
};
