#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

LiquidCrystal lcd (7,6,5,4,3,2);
Adafruit_NeoPixel leds(1, 13, NEO_GRB + NEO_KHZ800);

long readOneFSR(byte pin)
{
  long reading = analogRead(pin);
  long millivolts = map(reading, 0, 1023, 0, 5000);
  long newtons = 0;

  if (millivolts == 0)
  {
    return newtons;
  }

  // FSR = ((Vcc - V) * R) / V
  // R = 9.75 kOhm = 9750 Ohm
  // Vcc = 5 V = 5000 mV
  long resistance = ((5000 - millivolts) * 9750) / millivolts;
  long conductance = 1000000 / resistance;

  if (conductance < 1000){
    newtons = conductance / 80;
  } else {
    newtons = (conductance - 1000) / 30;
  }

  return newtons;
}

long readAllFSRs()
{
  long fsr1 = (readOneFSR(A0) + readOneFSR(A1)) / 2;
  long fsr2 = (readOneFSR(A2) + readOneFSR(A3)) / 2;
  long fsr = 0;
  bool have1 = fsr1 != 0;
  bool have2 = fsr1 != 0;

  if (!have1 && !have2)
  {
    return fsr;
  }
  else if (have1 && !have2)
  {
    fsr = fsr1;
  }
  else if (!have1 && have2)
  {
    fsr = fsr2;
  }
  else
  {
    fsr = (fsr1 + fsr2) / 2;
  }
  return fsr;
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Newtons:  Max:  ");
  leds.begin();
  leds.setPixelColor(0, leds.Color(0, 0, 0));  // Turn off LED
  leds.show();                        // Set new value
}

void loop() {
  long fsrNow = readAllFSRs();
  static long fsrMax = -1, fsrPrev = -1;

  if (fsrPrev == -1)
  {
  }
  else
  {
    fsrNow = 0.9 * fsrPrev + 0.1 *fsrNow;
  }
  fsrPrev = fsrNow;

  if (fsrNow > fsrMax)
  {
    fsrMax = fsrNow;
  }

  byte red = (fsrNow > 100) ? 255 : map (fsrNow,0,100,10,255); 
  byte lum = (fsrNow > 400) ? 255 : map (fsrNow,0,400,10,255);
  leds.setPixelColor(0,leds.Color(red,255-red,0));
  leds.setBrightness(lum);
  leds.show();
  delay(5);

  long millisNow = millis();
  static long millisLast = -1;

  if (millisNow - millisLast > 500)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(fsrNow);
    lcd.setCursor(10,1);
    lcd.print(fsrMax);

    millisLast = millisNow;
  }
}