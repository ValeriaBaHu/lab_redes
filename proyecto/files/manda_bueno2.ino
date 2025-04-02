/*ENVIO*/
// INCLUIR LIBRERIAS
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Crear una constante con la MAC
uint8_t broadcastAddress[] = {0x88, 0x13, 0xBF, 0x71, 0xCD, 0x48};

// Crear una estructura de información a enviar
typedef struct struct_msj {
  int servoPos;  // Variable para enviar la posición del servo (1 para 0 grados, 2 para 180 grados)
} struct_msj;

// Crear variable de tipo struct_msj
struct_msj datosEnviados;

// Crear nuevo objeto
esp_now_peer_info_t peerInfo;

Servo myServo;  // Crear un objeto para el servo

// Pines de los sensores
const int trigger1 = 12;
const int echo1 = 4;
const int trigger2 = 13;
const int echo2 = 9;

// Variables de control para evitar bucles
unsigned long ultimaAccion = 0;
int puertaAbierta = 0;  // 0 = cerrada, 1 = abierta a 0°, 2 = abierta a 180°

// Función para revisar si llegó bien la información
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);

  myServo.attach(15);
  myServo.write(90);  // Posición inicial
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
  long distance1 = getDistance(trigger1, echo1);
  long distance2 = getDistance(trigger2, echo2);

  Serial.print("Sensor 1 Distance: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Sensor 2 Distance: ");
  Serial.print(distance2);
  Serial.println(" cm");

  if (millis() - ultimaAccion > 5000) { // Solo permite una acción cada 5 segundos
    if (distance1 < 20 && distance2 >= 20 && puertaAbierta != 1) {
      myServo.write(0);
      puertaAbierta = 1;
      datosEnviados.servoPos = 1;
      ultimaAccion = millis();
    } 
    else if (distance2 < 20 && distance1 >= 20 && puertaAbierta != 2) {
      myServo.write(180);
      puertaAbierta = 2;
      datosEnviados.servoPos = 2;
      ultimaAccion = millis();
    }

    if (datosEnviados.servoPos != 0) {
      esp_now_send(broadcastAddress, (uint8_t *)&datosEnviados, sizeof(datosEnviados));
    }

    delay(5000);  // Espera con la puerta abierta
    myServo.write(90);  // Regresa a 90°
    puertaAbierta = 0;
    datosEnviados.servoPos = 0;
  }

  delay(500);  // Pequeño delay para evitar lecturas muy rápidas
}
