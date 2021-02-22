#pragma once

#include <Arduino.h>

#include "Potentiometer.h"
#include "Button.h"
#include "log.h"


class ClickablePotentiometer : public Potentiometer {
  public:
    int pinButton;
    unsigned long lastFlake;
    bool wasPressed = false;
    
    ClickablePotentiometer(int pinPotentiomer, int pinButton);
    void setup();
    void process();
    virtual void onChange(int value) = 0;
    virtual void onButtonPress();
    virtual void onButtonRelease();
};
