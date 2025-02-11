#include <DHT11.h>
//define pin data
#define pinDATA SDA // SDA, or almost any other I/O pin

DHT11 dht11(32); 
 
void setup() {
  Serial.begin(9600); //1bit=10µs
  Serial.println("\ntest capteur DTH22");
}

void loop() {
  //Serial.println(dht22.debug()); //optionnal

  float t = dht11.readTemperature();
  float h = dht11.readHumidity();

  /*if (dht22.getLastError() != dht22.OK) {
    Serial.print("last error :");
    Serial.println(dht22.getLastError());
  }
*/
  Serial.print("h=");Serial.print(h,1);Serial.print("\t");
  Serial.print("t=");Serial.println(t,1);
  delay(5000); //Collecting period should be : >1.7 second
}
