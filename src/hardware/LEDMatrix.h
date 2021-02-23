#pragma once

#include "log.h"
#include "BaseComponent.h"

class LEDMatrix : public BaseComponent {
  public:
    int* column_pins;
    int count_column_pins;
    int* row_pins;
    int count_row_pins;

    LEDMatrix(int* column_pins, int count_column_pins, int* row_pins, int count_row_pins);
    void setup();
    void setupRow(int row_pin);
    void setupColumn(int column_pin);
    void selectRow(int row_index);
    void unselectRow(int row_index);
    void turnOn(int column_pin);
    void turnOff(int column_pin);
    int digitalWriteColumn(int column_pin, int pin_state);
    void process();
};
