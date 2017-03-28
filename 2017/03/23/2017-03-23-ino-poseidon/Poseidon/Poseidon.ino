/**@file Poseidon.ino */
#include "Poseidon.hpp"
/*
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
*/

/* FSR testing sketch. 
 
Connect one end of FSR to power, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground 
 
For more information see www.ladyada.net/learn/sensors/fsr.html */


LiquidCrystal lcd(7,6,5,4,3,2);

void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
  lcd.begin(16,2);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
}

void loop(void) {
  int c = 0;
  lcd.setCursor(c++,0);c++;
  lcd.print(sizeof(byte));
  lcd.setCursor(c++,0);c++;
  lcd.print(sizeof(char));
  lcd.setCursor(c++,0);c++;
  lcd.print(sizeof(int));
  lcd.setCursor(c++,0);c++;
  lcd.print(sizeof(long));
  delay(100);
}
 
long newtons(int fsrPin) {// the FSR and 10K pulldown are connected to a0
 
int fsrReading;     // the analog reading from the FSR resistor divider
int fsrVoltage;     // the analog reading converted to voltage
unsigned long fsrResistance;  // The voltage converted to resistance, can be very big so make "long"
unsigned long fsrConductance; 
long fsrForce=0;       // Finally, the resistance converted to force

  fsrReading = analogRead(fsrPin);  
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
 
  // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
  Serial.print("Voltage reading in mV = ");
  Serial.println(fsrVoltage);  
 
  if (fsrVoltage == 0) {
    Serial.println("No pressure");  
  } else {
    // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
    // so FSR = ((Vcc - V) * R) / V        yay math!
    fsrResistance = 5000 - fsrVoltage;     // fsrVoltage is in millivolts so 5V = 5000mV
    fsrResistance *= 10000;                // 10K resistor
    fsrResistance /= fsrVoltage;
    Serial.print("FSR resistance in ohms = ");
    Serial.println(fsrResistance);
 
    fsrConductance = 1000000;           // we measure in micromhos so 
    fsrConductance /= fsrResistance;
    Serial.print("Conductance in microMhos: ");
    Serial.println(fsrConductance);
 
    // Use the two FSR guide graphs to approximate the force
    if (fsrConductance <= 1000) {
      fsrForce = fsrConductance / 80;
      Serial.print("Force in Newtons: ");
      Serial.println(fsrForce);      
    } else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      Serial.print("Force in Newtons: ");
      Serial.println(fsrForce);            
    }
  }
  Serial.println("--------------------");
  delay(1000);
  return fsrForce;
}

