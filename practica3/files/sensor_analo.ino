// Librerías necesarias
#include <DHT.h>       
#include <WiFi.h>      
#include <HTTPClient.h> 


int led = 32;  


DHT dht(33, DHT22); // DHTPIN (Pin 33), DHTTYPE (Sensor DHT22)

// Variables para almacenar los valores de temperatura y humedad
int valor_temperatura = 0;
int valor_humedad = 0;

String url = "https://api.thingspeak.com/update?"; // URL del servidor de ThingSpeak
String api_key = "L4E417INWL0WNQJ1"; // Clave API personal del tablero en ThingSpeak
String sensor = "field3"; // Campo donde se enviarán los datos de humedad

void setup() {
  
  pinMode(led, OUTPUT);  // Configura el pin del LED como salida
  dht.begin();           // Inicializa el sensor DHT22

  delay(1000); 
  Serial.begin(115200);  
    
  // Conexión a la red WiFi
  WiFi.begin("MEGACABLE-2.4G-E3A3", "JtsX8W7e24");  

  
  while ((WiFi.status() != WL_CONNECTED)) {
    delay(500);
    Serial.print(".");  
  }   
  Serial.println("WiFi conectado");  
}

void loop() {
  delay(1000); 

  
  valor_temperatura = dht.readTemperature();
  valor_humedad = dht.readHumidity();

  // Mostrar los valores en la consola serie
  Serial.print("Temperatura: ");
  Serial.println(valor_temperatura);
  Serial.print("Humedad: ");
  Serial.println(valor_humedad);

  // Encender el LED si la humedad es mayor o igual al 50%
  if (valor_humedad >= 50) { 
    digitalWrite(led, HIGH);  // Enciende el LED
  } else {
    digitalWrite(led, LOW);   // Apaga el LED
  }

  // Verifica si el ESP32 está conectado a WiFi antes de enviar datos
  if ((WiFi.status() == WL_CONNECTED)) {
    // Crear un objeto HTTPClient para la solicitud HTTP
    HTTPClient http;

    // Construir la URL con los datos de humedad
    String message = url + "api_key=" + api_key + "&" + sensor + "=" + String(valor_humedad);
    http.begin(message); // Iniciar la conexión HTTP

   
    int httpCode = http.GET();
    Serial.println(message); 
    Serial.print("Código GET: ");
    Serial.println(httpCode);  

    http.end();  
  }

  delay(1000);  
}
