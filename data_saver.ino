#include "weather_data.h"

WeatherData getCurrentWeatherData() {
  String timestamp = getCurrentTime();
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

String getWeatherCondition(WeatherData weatherData) {
  String weatherCondition;

  if (weatherData.temperature > 30) {
    weatherCondition = "Hot";
  } else if (weatherData.temperature > 20) {
    weatherCondition = "Warm";
  } else if (weatherData.temperature > 10) {
    weatherCondition = "Cool";
  } else {
    weatherCondition = "Cold";
  }

  if (weatherData.humidity > 70) {
    weatherCondition += " and Humid";
  } else if (weatherData.humidity < 30) {
    weatherCondition += " and Dry";
  }

  if (weatherData.humidity > 50 && weatherData.noise > 40) {
    weatherCondition += " and Windy";
  }

  if (isDaytimeNow()) {
    if (weatherData.brightness > 200) {
      weatherCondition += " - Sunny";
    } else if (weatherData.brightness < 20) {
      weatherCondition += " - Cloudy";
    } else {
      weatherCondition += " - Partly Cloudy";
    }
  }

  return weatherCondition;
}