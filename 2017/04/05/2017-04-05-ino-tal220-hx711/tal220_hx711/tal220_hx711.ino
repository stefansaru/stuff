
#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

#ifndef HX711_h
#define HX711_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class HX711
{
  private:
    byte PD_SCK;  // Power Down and Serial Clock Input Pin
    byte DOUT;    // Serial Data Output Pin
    byte GAIN;    // amplification factor
    long OFFSET = 0;  // used for tare weight
    float SCALE = 1;  // used to return weight in grams, kg, ounces, whatever

  public:
    // define clock and data pin, channel, and gain factor
    // channel selection is made by passing the appropriate gain: 128 or 64 for channel A, 32 for channel B
    // gain: 128 or 64 for channel A; channel B works with 32 gain factor only
    HX711(byte dout, byte pd_sck, byte gain = 128);

    HX711();

    virtual ~HX711();

    // Allows to set the pins and gain later than in the constructor
    void begin(byte dout, byte pd_sck, byte gain = 128);

    // check if HX711 is ready
    // from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
    // input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
    bool is_ready();

    // set the gain factor; takes effect only after a call to read()
    // channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
    // depending on the parameter, the channel is also set to either A or B
    void set_gain(byte gain = 128);

    // waits for the chip to be ready and returns a reading
    long read();

    // returns an average reading; times = how many times to read
    long read_average(byte times = 10);

    // returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
    double get_value(byte times = 1);

    // returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
    // times = how many readings to do
    float get_units(byte times = 1);

    // set the OFFSET value for tare weight; times = how many times to read the tare value
    void tare(byte times = 10);

    // set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
    void set_scale(float scale = 1.f);

    // get the current SCALE
    float get_scale();

    // set OFFSET, the value that's subtracted from the actual reading (tare weight)
    void set_offset(long offset = 0);

    // get the current OFFSET
    long get_offset();

    // puts the chip into power down mode
    void power_down();

    // wakes up the chip after power down mode
    void power_up();
};

#endif /* HX711_h */
//#include <Arduino.h>

#if ARDUINO_VERSION <= 106
    // "yield" is not implemented as noop in older Arduino Core releases, so let's define it.
    // See also: https://stackoverflow.com/questions/34497758/what-is-the-secret-of-the-arduino-yieldfunction/34498165#34498165
    void yield(void) {};
#endif

HX711::HX711(byte dout, byte pd_sck, byte gain) {
  begin(dout, pd_sck, gain);
}

HX711::HX711() {
}

HX711::~HX711() {
}

void HX711::begin(byte dout, byte pd_sck, byte gain) {
  PD_SCK = pd_sck;
  DOUT = dout;

  pinMode(PD_SCK, OUTPUT);
  pinMode(DOUT, INPUT);

  set_gain(gain);
}

bool HX711::is_ready() {
  return digitalRead(DOUT) == LOW;
}

void HX711::set_gain(byte gain) {
  switch (gain) {
    case 128:   // channel A, gain factor 128
      GAIN = 1;
      break;
    case 64:    // channel A, gain factor 64
      GAIN = 3;
      break;
    case 32:    // channel B, gain factor 32
      GAIN = 2;
      break;
  }

  digitalWrite(PD_SCK, LOW);
  read();
}

long HX711::read() {
  // wait for the chip to become ready
  while (!is_ready()) {
    // Will do nothing on Arduino but prevent resets of ESP8266 (Watchdog Issue)
    yield();
  }

  unsigned long value = 0;
  uint8_t data[3] = { 0 };
  uint8_t filler = 0x00;

  // pulse the clock pin 24 times to read the data
  data[2] = shiftIn(DOUT, PD_SCK, MSBFIRST);
  data[1] = shiftIn(DOUT, PD_SCK, MSBFIRST);
  data[0] = shiftIn(DOUT, PD_SCK, MSBFIRST);

  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < GAIN; i++) {
    digitalWrite(PD_SCK, HIGH);
    digitalWrite(PD_SCK, LOW);
  }

  // Replicate the most significant bit to pad out a 32-bit signed integer
  if (data[2] & 0x80) {
    filler = 0xFF;
  } else {
    filler = 0x00;
  }

  // Construct a 32-bit signed integer
  value = ( static_cast<unsigned long>(filler) << 24
      | static_cast<unsigned long>(data[2]) << 16
      | static_cast<unsigned long>(data[1]) << 8
      | static_cast<unsigned long>(data[0]) );

  return static_cast<long>(value);
}

long HX711::read_average(byte times) {
  long sum = 0;
  for (byte i = 0; i < times; i++) {
    sum += read();
    yield();
  }
  return sum / times;
}

double HX711::get_value(byte times) {
  return read_average(times) - OFFSET;
}

float HX711::get_units(byte times) {
  return get_value(times) / SCALE;
}

void HX711::tare(byte times) {
  double sum = read_average(times);
  set_offset(sum);
}

void HX711::set_scale(float scale) {
  SCALE = scale;
}

float HX711::get_scale() {
  return SCALE;
}

void HX711::set_offset(long offset) {
  OFFSET = offset;
}

long HX711::get_offset() {
  return OFFSET;
}

void HX711::power_down() {
  digitalWrite(PD_SCK, LOW);
  digitalWrite(PD_SCK, HIGH);
}

void HX711::power_up() {
  digitalWrite(PD_SCK, LOW);
}

//#define calibration_factor 3200 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define calibration_factor 13.565f

#define DOUT  11
#define CLK  9

HX711 scale(DOUT, CLK);

void setup_calibrate()
{
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  //scale.set_scale();
  scale.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0

  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Place weight on ");
  lcd.setCursor(0,1);
  lcd.print("the device (5s) ");
  delay(5000);
  lcd.setCursor(0,0);
  lcd.print("Value:          ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void setup_normal()
{
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0
  
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Newtons: GramsF:");
}

float absolute(float x)
{
  return x < 0 ? -x : x;
}

void loop_calibrate()
{
  float reading= scale.get_units(100);
  Serial.print(reading);
  lcd.setCursor(0,1);
  lcd.print(reading);
}

void loop_normal()
{
  //float grams = scale.get_units(2)* 0.0678f;
  float grams = scale.get_units(5)* 0.0628f;
  //float grams = scale.get_units(3);
  grams = absolute(grams) < 0.5f ? 0.0f : grams ;
  float newtons = grams * 0.0098f;
  static long lastUpdate = millis();
  static const long debounceMillis = 500;
  long currentTime = millis();
  
  if (currentTime - lastUpdate > debounceMillis)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(newtons);
    lcd.setCursor(9,1);
    lcd.print(grams);

    lastUpdate = currentTime;
  }

  
}

//bool calibrate = true;
bool calibrate = false;

void setup() {
  if (calibrate) setup_calibrate();
  else setup_normal();
}

void loop() {
  if (calibrate) loop_calibrate();
  else loop_normal();
}
/*
//HX711 scale(A1, A0); // parameter "gain" is ommited; the default value 128 is used by the library

void setup() {
Serial.begin(9600);
Serial.println("You Have 20s to Place a Known weight on the Sensor:"); // Time delay to place weight

delay(5000);

scale.set_scale(); // this value is obtained by dividing the calibration value with the known weight;
scale.tare(); // reset the scale to 0

Serial.println("Calibration Value:"); // divide this bby the weight of the load used
}

void loop() {
Serial.print("Divide this by value by the weight used in desired output units e.g. Grams/Kilograms:\t");
Serial.println(scale.get_units(10));

scale.power_down(); // put the ADC in sleep mode
delay(1000);
scale.power_up();
}
*/
