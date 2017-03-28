/**@file Configuration.hpp */
#pragma once
#include "Poseidon.hpp"

struct Configuration
{
  /*
   * Debounced button configuration
   */
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
  /*
   * Simple Led configuration
   */
  struct Led
  {
    enum
    {
      InitialState = LOW
    };
  };
  /*
   * RGB Led configuration
   */
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
  /*
   * Arduino LCD 16x2 shield configuration
   */
  class Lcd1602Shield
  {
    enum 
    {
        PIN_1 = 7
      , PIN_2 = 6
      , PIN_3 = 5
      , PIN_4 = 4
      , PIN_5 = 3
      , PIN_6 = 2
    };
    private:
      Lcd1602Shield() {}
      Lcd1602Shield(const Lcd1602Shield&);
      Lcd1602Shield& operator=(const Lcd1602Shield&);
    public:
      static LiquidCrystal& getDriver()
      {
        static LiquidCrystal singleton(PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6);
        return singleton;
      }
      static void setup()
      {
        static bool enabled = false;
        if (enabled) return;
        getDriver().begin(16,2);
        enabled = true;
      }
  };
};

