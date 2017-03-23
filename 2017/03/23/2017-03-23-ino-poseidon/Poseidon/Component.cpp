/**@file Component.cpp */
#include "Poseidon.hpp"

/// --
Component::Component ( int pin , Component::Mode mode , Component::Type type, int state) : 
      pin(pin), mode(mode), type(type), state(state)
{
  switch(type)
  {
    case Component::Type::Analog:
    {
      readCallback = &Component::readAnalog;
      writeCallback = &Component::writeAnalog;
    }; break;
    case Component::Type::Digital:
    {
      readCallback = &Component::readDigital;
      writeCallback = &Component::writeDigital;
    }; break;
  }
  switch(mode)
  {
    case Component::Mode::Input:
    {
      updateCallback = &Component::readState;
    }; break;
    case Component::Mode::Output:
    {
      updateCallback = &Component::writeState;
    }; break;
  }
}

/// --
void Component::setup()
{
  pinMode(pin, mode);
}
