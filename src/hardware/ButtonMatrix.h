#pragma once

#include "log.h"
#include "BaseComponent.h"
#include "Button.h"

class ButtonMatrix : public BaseComponent {
  public:
    int* column_pins;
    int count_column_pins;
    int* row_pins;
    int count_row_pins;
    
    int selected_row_index = -1;

    Button** buttons;

    ButtonMatrix(int* column_pins, int count_column_pins, int* row_pins, int count_row_pins);
    void setup();
    void setupRow(int row_pin);
    void setupColumn(int column_pin);
    void selectRow(int row_index);
    void unselectRow();
    ButtonState buttonStateColumn(int column_index);
    void process();
};
