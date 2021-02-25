#pragma once

#include "log.h"
#include "BaseComponent.h"

bool isBouncing(int pin_value, int* last_bounce_pin_value, unsigned long* last_bounce_millis);
enum class ButtonState {Pressed, Unchanged, Released};
ButtonState buttonState(int pin_value, bool* was_pressed,  unsigned long* last_bounce_millis, int* last_bounce_pin_value);
char* buttonStateToString(ButtonState buttonState);
bool buttonToggle(ButtonState buttonState, bool* toggle);

class Button : public BaseComponent {
  public:
    int pin_button; 
    bool button_was_pressed = false;
    unsigned long last_bounce_millis = -1;
    int last_bounce_pin_value;
    Button(int pin_button);
    void setup();
    void process();
    void _process(int pin_value);
    ButtonState _buttonState(int pin_value);
    virtual void onPress();
    virtual void onRelease();
};
