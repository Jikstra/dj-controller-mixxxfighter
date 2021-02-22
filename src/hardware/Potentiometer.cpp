#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin) : pin(pin) {}
  

void Potentiometer::setup() {}

void Potentiometer::process() {
  int value = analogRead(pin);

  // Smooth the analog value with some mathgic
  int smoothedDifference = (value - smoothedValue) / 4;
  if (smoothedDifference == 0) return;
  smoothedValue += smoothedDifference;

  //println("Potentiometer [%i] %i", pin, value);

  onChange(value);
}
