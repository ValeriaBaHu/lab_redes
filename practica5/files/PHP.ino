#include <WiFi.h>        // Librería para manejar WiFi
#include <HTTPClient.h>  // Librería para hacer solicitudes HTTP
#include <DHT.h>         // Librería para el sensor DHT22

// ------------------------ Puertos GPIO ------------------------
int pot = 32;   // Pin del potenciómetro
int boton = 33; // Pin del botón
int PIR = 26;   // Pin del sensor de movimiento (PIR)

// ------------------------ Configuración del sensor DHT22 ------------------------
DHT dht(25, DHT22); // Pin 25 con un DHT22

// ------------------------ Configuración de WiFi y ThingSpeak ------------------------
const char* ssid = "MEGACABLE-2.4G-E3A3";     // Nombre de la red WiFi
const char* password = "JtsX8W7e24";            // Contraseña (vacía en Wokwi)

String thinkspeakURL = "https://api.thingspeak.com/update?api_key=SAI2TW1H89YUA7U6";

void setup() {
  Serial.begin(115200);   
  dht.begin(); // Iniciar el sensor DHT22

  pinMode(boton, INPUT);
  pinMode(pot, INPUT);
  pinMode(PIR, INPUT);

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void loop() {
  // Leer valores de los sensores
  int valorBoton = digitalRead(boton);
  int valorPot = analogRead(pot);
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  int movimiento = digitalRead(PIR);

  // Validar si el sensor DHT22 proporciona valores correctos
  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error al leer el DHT22. Saltando envío...");
    return; // No enviamos datos si hay error
  }

  // Construir la URL de ThingSpeak con todos los sensores
  String url = thinkspeakURL 
               + "&field1=" + String(valorBoton)
               + "&field2=" + String(valorPot)
               + "&field3=" + String(temperatura)
               + "&field4=" + String(humedad)
               + "&field5=" + String(movimiento);

  // Enviar los datos a ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    // Mostrar la URL y el código de respuesta en el monitor serie
    Serial.println(url);
    Serial.print("Código de respuesta: ");
    Serial.println(httpCode);

    http.end();
  }

  // Esperar 15 segundos antes del próximo envío (ThingSpeak limita a 15s)
  delay(15000);
}
