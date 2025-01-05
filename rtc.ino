#include <Ds1302.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#define UTC2 3600 * 2

WiFiEspUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC2, 60000);

Ds1302 rtc(4, 2, 3);

void setupRTC() {
  rtc.init();
  if (rtc.isHalted()) {
    Serial.println("RTC is halted. Setting time...");

    timeClient.begin();
    while (!timeClient.update()) {
      timeClient.forceUpdate();
    }

    time_t rawTime = timeClient.getEpochTime();
    setTime(rawTime);

    Ds1302::DateTime dateTime = {
        .year = static_cast<uint8_t>(year() - 2000),
        .month = static_cast<uint8_t>(month()),
        .day = static_cast<uint8_t>(day()),
        .hour = static_cast<uint8_t>(hour()),
        .minute = static_cast<uint8_t>(minute()),
        .second = static_cast<uint8_t>(second()),
        .dow = static_cast<uint8_t>(weekday())
    };

    rtc.setDateTime(&dateTime);
    Serial.println("RTC time updated!");
  } else {
    Serial.println("RTC works correctly.");
  }
}

String formatTwoDigits(int number) {
  return (number < 10) ? "0" + String(number) : String(number);
}

String getCurrentTime() {
  Ds1302::DateTime now;
  rtc.getDateTime(&now);
  return formatTwoDigits(now.day) + "/" + formatTwoDigits(now.month) + "/" + String(now.year) + " " +
         formatTwoDigits(now.hour) + ":" + formatTwoDigits(now.minute) + ":" + formatTwoDigits(now.second);
}

bool isDaytimeNow() {
  Ds1302::DateTime now;
  rtc.getDateTime(&now);
  return now.hour > 7 && now.hour < 19 ? true : false;
}