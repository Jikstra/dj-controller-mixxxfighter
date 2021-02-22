#include "main.h"

Matrix LEDMatrix = new Matrix({ A0, A1 }, { A2, A3, A4, A5});

BaseComponent* components[] = {
  

};

void setup() { 
  Serial.begin(115200);
  LEDMatrix.setup();
}


void loop() {
}
