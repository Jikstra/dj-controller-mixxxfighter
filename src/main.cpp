#include "main.h"


const int CountLEDMatrixColumns = 4;
int LEDMatrixColumns[CountLEDMatrixColumns] = { A2, A3, A4, A5};
const int CountLEDMatrixRows = 2;
int LEDMatrixRows[CountLEDMatrixRows] = { A0, A1 };
LEDMatrix* led_matrix = new LEDMatrix(LEDMatrixColumns, CountLEDMatrixColumns, LEDMatrixRows, CountLEDMatrixRows);

BaseComponent* components[] = {
  

};

void setup() { 
  Serial.begin(115200);
  led_matrix->setup();
}


void loop() {
  for (int row_index = 0; row_index < CountLEDMatrixRows; row_index++) {
    led_matrix->selectRow(row_index);
    for (int column_index = 0; column_index < CountLEDMatrixColumns; column_index++) {
      led_matrix->turnOn(column_index);
      delay(1);
      led_matrix->turnOff(column_index);
    }
    led_matrix->unselectRow(row_index);
  }
}
