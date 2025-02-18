#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

void setup()
{
  Serial.begin(9600);
  delay(2000);
  dht.begin();
}

void loop()
{
  delay(2000);

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) 
  {
    Serial.println("ERREUR de lecture !");
    return;
  }

  Serial.println("Temperature : ");
  Serial.println(t);
  Serial.println("°C");
  Serial.println("Humi");
  Serial.println(h);
  Serial.println("%");
}