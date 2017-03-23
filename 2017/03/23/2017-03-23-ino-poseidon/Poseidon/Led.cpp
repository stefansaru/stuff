/**@file Led.cpp */
#include "Poseidon.hpp"

Led::Led(int pin) : 
    Component(
        pin = pin
      , mode = Component::Mode::Output
      , type = Component::Type::Digital
      , state = Configuration::Led::InitialState
    )
{ }
