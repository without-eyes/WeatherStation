#include <TimeLib.h>
#include "weather_data.h"

WeatherData getCurrentWeatherData() {
  String timestamp = String(hour()) + ":" + String(minute()) + ":" + String(second()) + " " +
                     String(day()) + "/" + String(month()) + "/" + String(year());
  readFromDHT();

  WeatherData weatherData = {
    timestamp,
    DHT.temperature,
    DHT.humidity,
    getBrightness(),
    getNoise()
  };
  
  return weatherData;
}