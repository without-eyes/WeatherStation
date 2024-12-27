#include <dht.h>
#define DHT11_PIN 7
#define PHOTORESISTOR_PIN A0

dht DHT;

void setup() {
  Serial.begin(9600);
}

void loop() {
  DHT.read11(DHT11_PIN);
  Serial.print("DHT: Temperature = ");
  Serial.print(DHT.temperature);
  Serial.print(";   Humidity = ");
  Serial.println(DHT.humidity);

  int brightness = analogRead(A0);
  Serial.print("Photoresistor: brightness = ");
  Serial.println(brightness);

  delay(2000);
}
