/**@file Component.hpp */
#pragma once
#include "Poseidon.hpp"

/*! A single pin component base class */
class Component
{
public:
  enum Type { Analog , Digital };
  enum Mode { Input, Output};
  
protected:
  int pin;
  Mode mode;
  Type type;
  int state;
  
public:
  Component ( int pin , Mode mode , Type type, int state );
  
protected:
  inline int read() { return (this->*readCallback)(); }
  int (Component::*readCallback)();
  inline int readAnalog() { return analogRead(pin); }
  inline int readDigital() { return digitalRead(pin); }

  inline void write(int value) { (this->*writeCallback)(value); }
  void (Component::*writeCallback)(int);
  inline void writeAnalog(int value) { return analogWrite(pin, value); }
  inline void writeDigital(int value) { return digitalWrite(pin, value); }

public:
  virtual void setup();
  virtual void update() { return (this->*updateCallback)(); }
  void (Component::*updateCallback)();
  inline void readState() { state = read(); }
  inline void writeState() { write(state); }
};

