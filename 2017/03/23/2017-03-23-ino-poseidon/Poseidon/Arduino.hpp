/**@file Arduino.cpp */
#pragma once
/*
 * extern void pinMode(int,int);
 * extern int digitalRead(int);
 * extern void digitalWrite(int,int);
 * extern int analogRead(int);
 * extern void analogWrite(int,int);
 * extern unsigned long millis(void);
 */
#include "Arduino.h"
/*
 * WS2812 RGB Led Driver
 */
#include <Adafruit_NeoPixel.h>
/*
 * LCD 1602 16 columns 2 lines 
 */
#include <LiquidCrystal.h>


typedef int (*CallbackRead) (int);
typedef void (*CallbackWrite) (int,int);

