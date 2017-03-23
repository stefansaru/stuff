/**@file Poseidon.ino */
#include "Poseidon.hpp"

void setup() {
  // put your setup code here, to run once:
  System::Instance()
    .setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  System::Instance()
    .loop();
}
