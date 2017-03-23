/**@file Buton.cpp */
#include "Poseidon.hpp"

Button::Button(int pin) : 
    Component(
        pin = pin
      , mode = Component::Mode::Input
      , type = Component::Type::Digital
      , state = Configuration::Button::InitialState
    )
    , statePrevious(Configuration::Button::InitialStatePrevious)
    , lastDebounceTime(Configuration::Button::InitialLastDebounceTime)
    , debounceDelay(Configuration::Button::InitialDebounceDelay)
{ }

void Button::update()
{
  int currentTime = millis();
  int sample = read();

  if (sample != statePrevious)
    lastDebounceTime = currentTime;

  if (currentTime - lastDebounceTime > debounceDelay)
    if (state != sample)
      state = sample;

  statePrevious = sample;
}
