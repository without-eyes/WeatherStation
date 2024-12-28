#include <dht.h>
#define DHT11_PIN 7
#define PHOTORESISTOR_PIN A0
#define MICROPHONE_PIN A1

int readMicrophone() {
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += analogRead(A1);
  }
  return sum / 50;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  dht DHT;
  DHT.read11(DHT11_PIN);
  Serial.print("DHT: Temperature = ");
  Serial.print(DHT.temperature);
  Serial.print(";   Humidity = ");
  Serial.println(DHT.humidity);

  int brightness = analogRead(A0);
  Serial.print("Photoresistor: brightness = ");
  Serial.println(brightness);

  int noise = readMicrophone();
  Serial.print("Microphone: noise level = ");
  Serial.println(noise);

  Serial.println("");

  delay(1000);
}
