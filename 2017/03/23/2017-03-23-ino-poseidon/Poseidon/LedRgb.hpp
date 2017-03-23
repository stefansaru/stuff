/**@file LedRgb.hpp */
#pragma once
#include "Poseidon.hpp"

class LedRgb : protected Led
{
public:
  LedRgb(int pin) : Led(pin) { }
  
  void setup() override
  {
    Component::setup(); ///< set pin direction
    Configuration::LedRgb::setup();///< enable leds
  }
  void update() override
  {
    write(state);
  }
};

