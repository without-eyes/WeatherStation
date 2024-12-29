#include "WiFiEsp.h"

void setup() {
  Serial.begin(115200);
  startServer();
}

void loop() {
  WiFiEspClient client = getClient();
  if (!client) return;

  boolean currentLineIsBlank = true;
  while (client.connected()) {
    if (!client.available()) continue;

    char c = client.read();
    Serial.write(c);
    if (c == '\n' && currentLineIsBlank) {
      sendHeaderAndHtmlPage(client);
      break;
    }
    if (c == '\n') {
      currentLineIsBlank = true;
    } else if (c != '\r') {
      currentLineIsBlank = false;
    }
  }
  delay(10);
  client.stop();
  Serial.println("Client disconnected");
}