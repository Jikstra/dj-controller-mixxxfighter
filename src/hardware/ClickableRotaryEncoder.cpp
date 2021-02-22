#include "ClickableRotaryEncoder.h"

ClickableRotaryEncoder::ClickableRotaryEncoder(int pin_ccw, int pin_button, int pin_cw) : RotaryEncoder(pin_ccw, pin_cw), pin_button(pin_button) {
}

void ClickableRotaryEncoder::setup() {
  RotaryEncoder::setup();
  pinMode(pin_button, INPUT_PULLUP);
}

void ClickableRotaryEncoder::process() {
  RotaryEncoder::process();

  int pin_value = digitalRead(pin_button);

  ButtonState button_state = buttonState(pin_value, &button_was_pressed, &button_last_flake);

  if(button_state == ButtonState::Pressed) onButtonPress();
  if(button_state == ButtonState::Released) onButtonRelease();
}

void ClickableRotaryEncoder::onButtonPress() {
  println("ClickableRotaryEncoder [%i] Pressed!", pin_button);
}

void ClickableRotaryEncoder::onButtonRelease() {
  println("ClickableRotaryEncoder [%i] Released!", pin_button);
}
