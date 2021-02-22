#pragma once

#include "log.h"
#include "BaseComponent.h"

class Matrix : public BaseComponent {
  public:
    int column_pins[];
    int row_pins[];

    Matrix(int column_pins[], int row_pins[]);
    void setup();
    void process();
};
