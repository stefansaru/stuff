/**@file Arduino.cpp */
#pragma once
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

/*
 * extern void pinMode(int,int);
 * extern int digitalRead(int);
 * extern void digitalWrite(int,int);
 * extern int analogRead(int);
 * extern void analogWrite(int,int);
 * extern unsigned long millis(void);
 */

typedef int (*CallbackRead) (int);
typedef void (*CallbackWrite) (int,int);

