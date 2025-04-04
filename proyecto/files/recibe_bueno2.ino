#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

#define BUZZER_PIN 5

const char* ssid = "MEGACABLE-2.4G-E3A3";
const char* password = "JtsX8W7e24";

WebServer server(80);

// ThingSpeak
String api_key = "VNBELU74MLET4LS1";
String base_url = "https://api.thingspeak.com/update?";

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Conectando al WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  server.on("/update", handleUpdate);
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();
}

void handleUpdate() {
  if (server.hasArg("pos")) {
    String pos = server.arg("pos");
    Serial.println("Recibido pos = " + pos);

    if (pos == "1" || pos == "2") {
      tone(BUZZER_PIN, 1000);
      delay(500);
      noTone(BUZZER_PIN);

      enviarAThingSpeak(pos.toInt());
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Valor no válido");
    }
  } else {
    server.send(400, "text/plain", "Falta parámetro 'pos'");
  }
}

void enviarAThingSpeak(int valor) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = base_url + "api_key=" + api_key + "&field1=" + String(valor);
    http.begin(url);
    int httpCode = http.GET();
    Serial.print("ThingSpeak response: ");
    Serial.println(httpCode);
    http.end();
  }
}
