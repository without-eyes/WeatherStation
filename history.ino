#include "weather_data.h"
#define MAX_RECORDS 72
#define RECORD_INTERVAL 3600000

WeatherData history[MAX_RECORDS];
int currentIndex = 0;
unsigned long lastRecordTime = 0;

void saveWeatherDataInHistory(WeatherData weatherData) {
  history[currentIndex] = weatherData;
  currentIndex = (currentIndex + 1) % MAX_RECORDS;
}

String getHistoricalData() {
  String historicalData = "";
  for (int i = 0; i < MAX_RECORDS; i++) {
    if (history[i].timestamp == "") continue;

    historicalData += "<tr><td>" + history[i].timestamp + "</td><td>" 
                      + String(history[i].temperature) + "\xB0""C</td><td>" 
                      + String(history[i].humidity) + "%</td><td>" 
                      + String(history[i].brightness) + " lux</td><td>" 
                      + String(history[i].noise) + " dB</td></tr>";
  }
  return historicalData;
}

bool hasHourPassed() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastRecordTime >= RECORD_INTERVAL) {
    lastRecordTime = currentMillis;
    return true;
  }

  return false;
}