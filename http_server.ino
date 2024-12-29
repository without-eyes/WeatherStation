#include "WiFiEsp.h"
#define HTTP_PORT 80
#define TIME_TO_REFRESH_IN_SECS 60

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
  readFromDHT();
  client.print("<!DOCTYPE HTML>\r\n");
  client.print("<html>\r\n");
  client.print("DHT: Temperature = ");
  client.print(DHT.temperature);
  client.print(";   Humidity = ");
  client.print(DHT.humidity);
  client.print("<br>\r\n");
  client.print("Photoresistor: brightness = ");
  client.print(getBrightness());
  client.print("<br>\r\n");
  client.print("Microphone: noise level = ");
  client.print(getNoise());
  client.print("<br>\r\n");
  client.print("</html>\r\n");
}