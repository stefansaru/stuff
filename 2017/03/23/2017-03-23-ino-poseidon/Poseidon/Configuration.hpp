/**@file Configuration.hpp */
#pragma once
#include "Poseidon.hpp"

struct Configuration
{
  struct Button
  {
    enum
    {
        InitialLastDebounceTime = 0
      , InitialState = LOW
      , InitialStatePrevious = LOW
      , InitialDebounceDelay = 50 ///< milliseconds
    };
  };
  struct Led
  {
    enum
    {
      InitialState = LOW
    };
  };
  class LedRgb
  {
    private:
      LedRgb() {}
      LedRgb(const LedRgb&);
      LedRgb& operator=(const LedRgb&);
    public:
      static Adafruit_NeoPixel& getDriver()
      {
        static Adafruit_NeoPixel singleton;
        return singleton;
      }
      static void setup()
      {
        static bool enabled = false;
        if (enabled) return;
        getDriver().begin();
        enabled = true;
      }
  };
};

