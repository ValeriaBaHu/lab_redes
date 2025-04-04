/*ENVIO*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// WiFi
const char* ssid = "MEGACABLE-2.4G-E3A3";
const char* password = "JtsX8W7e24";

// IP del ESP receptor
const char* receptor_ip = "192.168.100.69";  
Servo myServo;

// Pines de sensores
const int trigger1 = 12;
const int echo1 = 4;
const int trigger2 = 13;
const int echo2 = 9;

unsigned long ultimaAccion = 0;
int puertaAbierta = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Conectando al WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");

  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);

  myServo.attach(15);
  myServo.write(90);
}

long getDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.0343 / 2;
}

void loop() {
  long d1 = getDistance(trigger1, echo1);
  long d2 = getDistance(trigger2, echo2);

  Serial.print("Sensor 1: ");
  Serial.print(d1);
  Serial.print(" cm\tSensor 2: ");
  Serial.print(d2);
  Serial.println(" cm");

  if (millis() - ultimaAccion > 5000) {
    int pos = 0;

    if (d1 < 20 && d2 >= 20 && puertaAbierta != 1) {
      myServo.write(0);
      pos = 1;
      puertaAbierta = 1;
    } else if (d2 < 20 && d1 >= 20 && puertaAbierta != 2) {
      myServo.write(180);
      pos = 2;
      puertaAbierta = 2;
    }

    if (pos != 0) {
      enviarAlServidor(pos);
      ultimaAccion = millis();

      delay(5000); // mantener la puerta abierta
      myServo.write(90);
      puertaAbierta = 0;
    }
  }

  delay(500);
}

void enviarAlServidor(int pos) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://" + String(receptor_ip) + "/update?pos=" + String(pos);
    http.begin(url);
    int httpCode = http.GET();
    Serial.print("Enviado al servidor, código HTTP: ");
    Serial.println(httpCode);
    http.end();
  } else {
    Serial.println("Error: No conectado al WiFi");
  }
}
