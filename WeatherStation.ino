#include "WiFiEsp.h"
#include <dht.h>
#define DHT11_PIN 7
#define PHOTORESISTOR_PIN A0
#define MICROPHONE_PIN A1

char ssid[] = "wifi_name";
char pass[] = "password";
int status = WL_IDLE_STATUS;

dht DHT;
WiFiEspServer server(80);

int readMicrophone() {
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += analogRead(A1);
  }
  return sum / 50;
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  Serial.println();
  Serial.print("Weather Station page: http://");
  Serial.println(ip);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
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

void loop() {
  WiFiEspClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          DHT.read11(DHT11_PIN);
          unsigned short int brightness = analogRead(A0);
          unsigned short int noise = readMicrophone();

          Serial.println("Sending response");
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "Refresh: 2\r\n"
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("DHT: Temperature = ");
          client.print(DHT.temperature);
          client.print(";   Humidity = ");
          client.print(DHT.humidity);
          client.print("<br>\r\n");
          client.print("Photoresistor: brightness = ");
          client.print(brightness);
          client.print("<br>\r\n");
          client.print("Microphone: noise level = ");
          client.print(noise);
          client.print("<br>\r\n");
          client.print("</html>\r\n");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}
