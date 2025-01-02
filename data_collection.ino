#include <dht.h>

#define DHT11_PIN 7
#define PHOTORESISTOR_PIN A0
#define MICROPHONE_PIN A1

struct WeatherData {
  unsigned long timestamp;
  short temperature;
  unsigned short humidity;
  unsigned short brightness;
  unsigned short noise;
};

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

struct WeatherData getCurrentWeatherData() {
  readFromDHT();
  
  struct WeatherData weatherData;
  weatherData.timestamp = millis() / 1000;
  weatherData.temperature = DHT.temperature;
  weatherData.humidity = DHT.humidity;
  weatherData.brightness = getBrightness();
  weatherData.noise = getNoise();
  
  return weatherData;
}