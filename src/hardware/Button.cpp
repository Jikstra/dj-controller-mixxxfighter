#include "Button.h"

bool isBouncing(unsigned long* last_flake_millis) {
  unsigned long current_flake = millis();
  //println("current_flake %lu last_flake %lu", current_flake, *last_flake_millis);
  bool returnValue = current_flake - *last_flake_millis < 50;
  *last_flake_millis = current_flake;
  return returnValue;  
}


ButtonState buttonState(int pin_value, bool* was_pressed,  unsigned long* last_flake_millis) {
  if(pin_value == LOW && *was_pressed == false) {
    if(isBouncing(last_flake_millis)) return ButtonState::Unchanged;
    *was_pressed = true;
    return ButtonState::Pressed;
  } else if(pin_value == HIGH && *was_pressed == true){
    if(isBouncing(last_flake_millis)) return ButtonState::Unchanged;
    *was_pressed = false;
    return ButtonState::Released;
  } else {
    return ButtonState::Unchanged;
  }
}

char* buttonStateToString(ButtonState button_state) {
  switch(button_state) {
    case ButtonState::Pressed:
      return "Pressed";
    case ButtonState::Unchanged:
      return "Unchanged";
    case ButtonState::Released:
      return "Released";
    default:
      IFDEBUG(println("Invalid: %i", button_state));
      return "Invalid Enum";
      
  }
}

bool buttonToggle(ButtonState button_state, bool* toggle) {
  if(button_state == ButtonState::Pressed) {
    *toggle = !*toggle;
    return true;
  }
  return false;
}


Button::Button(int pin_button) : pin_button(pin_button) {}

void Button::setup() {
  pinMode(pin_button, INPUT_PULLUP);
}

void Button::process() {
  int pin_value = digitalRead(pin_button);

  _process(pin_value);
}

void Button::_process(int pin_value) {
  ButtonState button_state = _buttonState(pin_value);
  if(button_state == ButtonState::Pressed) onPress();
  if(button_state == ButtonState::Released) onRelease();
}

ButtonState Button::_buttonState(int pin_value) {
  ButtonState button_state = buttonState(pin_value, &button_was_pressed, &button_last_flake);
  return button_state;
}

void Button::onPress() {
  DBG("[Button] %i Pressed!", pin_button);
}
void Button::onRelease() {
  DBG("[Button] %i Released!", pin_button);
}
