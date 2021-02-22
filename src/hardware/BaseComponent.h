#pragma once

class BaseComponent {
  public:
    virtual void setup() = 0;
    virtual void process() = 0;
};
