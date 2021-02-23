#pragma once

#include "log.h"
#include "BaseComponent.h"

class LEDMatrix : public BaseComponent {
  public:
    int* column_pins;
    int count_column_pins;
    int* row_pins;
    int count_row_pins;
    
    int selected_row_index = -1;
    int turned_on_column_index = -1;

    LEDMatrix(int* column_pins, int count_column_pins, int* row_pins, int count_row_pins);
    void setup();
    void setupRow(int row_pin);
    void setupColumn(int column_pin);
    void selectRow(int row_index);
    void unselectRow();
    void turnOn(int column_index);
    void turnOff();
    int digitalWriteColumn(int column_pin, int pin_state);
    void process();
};
