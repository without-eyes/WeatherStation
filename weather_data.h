#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <Arduino.h>

typedef struct {
  String timestamp;
  float temperature;
  float humidity;
  unsigned short brightness;
  unsigned short noise;
} WeatherData;

#endif