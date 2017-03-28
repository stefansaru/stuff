/**@file System.hpp */
#pragma once
#include "Poseidon.hpp"

class System
{
private:
  System() { }
  System(const System&);
  System& operator=(const System&);
  
public:
  inline static System& Instance()
  {
    static System singleton; 
    return singleton;
  }
  inline void setup()
  {
    Configuration::Lcd1602Shield::setup();
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);
  }
  inline void loop()
  {
    Configuration::Lcd1602Shield::getDriver().setCursor(0,0);
    Configuration::Lcd1602Shield::getDriver().print(analogRead(A0));
    Configuration::Lcd1602Shield::getDriver().setCursor(0,1);
    Configuration::Lcd1602Shield::getDriver().print("00000");
    Configuration::Lcd1602Shield::getDriver().print(analogRead(A1));
    delay(1000);
  }
};

