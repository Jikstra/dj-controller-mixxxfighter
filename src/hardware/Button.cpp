#include "Button.h"

bool isBouncing(int pin_value, int* last_bounce_pin_value, unsigned long* last_bounce_millis) {
  if (*last_bounce_pin_value == pin_value) {
    return false;
  }
  unsigned long current_flake_millis = millis();
  //println("current_flake %lu last_flake %lu", current_flake, *last_bounce_millis);
  bool isBouncing = (current_flake_millis - *last_bounce_millis) < 50 ? true : false;
  if (isBouncing) {
    *last_bounce_millis = current_flake_millis;
  }
  *last_bounce_pin_value = pin_value;
  return isBouncing;
}


ButtonState buttonState(int pin_value, bool* was_pressed,  unsigned long* last_bounce_millis, int* last_bounce_pin_value) {
  if(pin_value == LOW && *was_pressed == false) {
    if(isBouncing(pin_value, last_bounce_pin_value, last_bounce_millis)) return ButtonState::Unchanged;
    *was_pressed = true;
    return ButtonState::Pressed;
  } else if(pin_value == HIGH && *was_pressed == true){
    bool is_bouncing = isBouncing(pin_value, last_bounce_pin_value, last_bounce_millis);
    DBG("isBouncing %d", isBouncing);
    if(is_bouncing) return ButtonState::Unchanged;
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
  ButtonState button_state = buttonState(pin_value, &button_was_pressed, &last_bounce_millis, &last_bounce_pin_value);
  return button_state;
}

void Button::onPress() {
  DBG("[Button] %i Pressed!", pin_button);
}
void Button::onRelease() {
  DBG("[Button] %i Released!", pin_button);
}
