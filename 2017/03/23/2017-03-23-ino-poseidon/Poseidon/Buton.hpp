/**@file Buton.hpp */
#include "Poseidon.hpp"

class Button : protected Component
{
  int statePrevious;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;

  Button(int pin);

  void update() override;
};


