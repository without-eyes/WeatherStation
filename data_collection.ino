#include <dht.h>

#define DHT11_PIN 7
#define PHOTORESISTOR_PIN A0
#define MICROPHONE_PIN A1

dht DHT;

void readFromDHT() {
  DHT.read11(DHT11_PIN);
}

unsigned short getBrightness() {
  return analogRead(PHOTORESISTOR_PIN);
}

unsigned short getNoise() {
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += analogRead(MICROPHONE_PIN);
  }
  return sum / 50;
}