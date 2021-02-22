#pragma once

#include "log.h"
#include "BaseComponent.h"

bool isBouncing(unsigned long* last_flake_millis);
enum class ButtonState {Pressed, Unchanged, Released};
ButtonState buttonState(int pinValue, bool* wasPressed,  unsigned long* lastFlakeMillis);
char* buttonStateToString(ButtonState buttonState);
bool buttonToggle(ButtonState buttonState, bool* toggle);

class Button : public BaseComponent {
  public:
    int pin_button; 
    bool button_was_pressed = false;
    unsigned long button_last_flake = -1;

    Button(int pin_button);
    void setup();
    void process();
    virtual void onPress();
    virtual void onRelease();
};
