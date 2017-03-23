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
    
  }
  inline void loop()
  {
    
  }
};

