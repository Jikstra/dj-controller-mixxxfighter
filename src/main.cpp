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

BaseComponent* components[] = {
};

void setup() { 
  Serial.begin(115200);
  led_matrix->setup();
  button_matrix->setup();
}


void loop() {
  /*for (int row_index = 0; row_index < CountLEDMatrixRows; row_index++) {
    led_matrix->selectRow(row_index);
    for (int column_index = 0; column_index < CountLEDMatrixColumns; column_index++) {
      led_matrix->turnOn(column_index);
      delay(1);
      led_matrix->turnOff(column_index);
    }
    led_matrix->unselectRow(row_index);
  }*/
  
  for (int row_index = 0; row_index < CountButtonMatrixRows; row_index++) {
    button_matrix->selectRow(row_index);
    for (int column_index = 0; column_index < CountButtonMatrixColumns; column_index++) {
      ButtonState button_state = button_matrix->buttonStateColumn(column_index);
      if (button_state == ButtonState::Unchanged) continue;
      DBG("%i:%i = %s", row_index, column_index, buttonStateToString(button_state));
    }
    button_matrix->unselectRow();
  }
}
