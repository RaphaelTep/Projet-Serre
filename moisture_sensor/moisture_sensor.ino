/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-soil-moisture-sensor
 */

#define AOUT_PIN 34// ESP32 pin GPIO36 (ADC0) that connects to AOUT pin of moisture sensor
#define THRESHOLD 500000 // CHANGE YOUR THRESHOLD HERE

void setup() {
  Serial.begin(9600);
  // set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);
}

void loop() {
  int value = analogRead(AOUT_PIN); // read the analog value from sensor

  if (value < THRESHOLD)
  {
    Serial.print("The soil is DRY (");
    Serial.print(value);
  }
  else
  {
    Serial.print("The soil is WET (");
    Serial.print(value);
  }

  Serial.print(value);
  Serial.println(")");


  delay(5000);
}