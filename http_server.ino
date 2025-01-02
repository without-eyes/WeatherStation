#include "WiFiEsp.h"
#include "weather_data.h"
#define HTTP_PORT 80
#define TIME_TO_REFRESH_IN_SECS 10

WiFiEspServer server(HTTP_PORT);
int status = WL_IDLE_STATUS;

void startServer() {
  Serial3.begin(115200);
  WiFi.init(&Serial3);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();

  server.begin();
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  Serial.println();
  Serial.print("Weather Station data: http://");
  Serial.println(ip);
  Serial.println();
}

WiFiEspClient getClient() {
  return server.available();
}

void sendHeaderAndHtmlPage(WiFiEspClient client) {
  sendHeader(client);
  sendHtmlPage(client);
}

void sendHeader(WiFiEspClient client) {
  Serial.println("Sending response");
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: text/html\r\n");
  client.print("Connection: close\r\n");
  client.print("Refresh: ");
  client.print(TIME_TO_REFRESH_IN_SECS);
  client.print("\r\n");
  client.print("\r\n");
}


void sendHtmlPage(WiFiEspClient client) {
  WeatherData weatherData = getCurrentWeatherData();
  client.print(generateHtmlPage(weatherData));
}

String generateHtmlPage(WeatherData weatherData) {
  String weatherCondition = "Sunny";

  String htmlPage = F(
    "<!DOCTYPE HTML>"
    "<html>"
    "<head>"
    "<title>Weather Station</title>"
    "<style>"
    "body { font-family: Arial, sans-serif; background-color: #f4f4f4; color: #333; margin: 0; padding: 0; display: flex; flex-direction: column; align-items: center; }"
    "h1 { background-color: #4CAF50; color: white; padding: 20px 0; margin: 0; width: 100%; text-align: center; }"
    ".content { display: flex; justify-content: space-between; width: 80%; margin: 20px auto; }"
    ".left, .right { background: #fff; padding: 20px; box-shadow: 0 0 10px rgba(0,0,0,0.1); border-radius: 8px; }"
    ".left { width: 45%; }"
    ".right { width: 45%; }"
    ".data { font-size: 18px; margin: 10px 0; }"
    "table { width: 100%; border-collapse: collapse; margin-top: 10px; }"
    "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }"
    "th { background-color: #4CAF50; color: white; }"
    "</style>"
    "</head>"
    "<body>"
    "<h1>Weather Station</h1>"
    "<div class='content'>"
    "<div class='left'>"
    "<h2>Current Weather</h2>"
  );

  htmlPage += "<div class='data'><strong>Condition:</strong> " + weatherCondition + "</div>";
  htmlPage += "<div class='data'><strong>Temperature</strong> = " + String(weatherData.temperature) + " \xB0""C</div>";
  htmlPage += "<div class='data'><strong>Humidity</strong> = " + String(weatherData.humidity) + "%</div>";
  htmlPage += "<div class='data'><strong>Brightness</strong> = " + String(weatherData.brightness) + " lux</div>";
  htmlPage += "<div class='data'><strong>Noise Level</strong> = " + String(weatherData.noise) + " dB</div>";
  
  htmlPage += F(
    "</div>"
    "<div class='right'>"
    "<h2>Historical Data (Last 3 Days)</h2>"
    "<table>"
    "<tr><th>Date</th><th>Temperature</th><th>Humidity</th><th>Brightness</th><th>Noise</th></tr>"
  );

  htmlPage += getHistoricalData();

  htmlPage += F(
    "</table>"
    "</div>"
    "</div>"
    "</body>"
    "</html>"
  );

  return htmlPage;
}