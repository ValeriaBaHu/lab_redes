#include <esp_now.h>
#include <WiFi.h>

#define BUZZER_PIN 5  // Definir el pin donde está conectado el buzzer

typedef struct struct_msj {
    int servoPos;  // Se recibe solo la posición del servo (1 o 2)
} struct_msj;

struct_msj datosRecibidos;

// Función callback en recepción de datos
// Imprime los datos recibidos y activa el buzzer si el valor recibido es 1 o 2
void OnDataRecv(const esp_now_recv_info* recv_info, const uint8_t *tempData, int tam) {
  memcpy(&datosRecibidos, tempData, sizeof(datosRecibidos));
  
  Serial.print("Bytes recibidos: ");
  Serial.println(tam);
  Serial.print("Posición del Servo: ");
  Serial.println(datosRecibidos.servoPos);

  // Activar el buzzer si se recibe 1 o 2
  if (datosRecibidos.servoPos == 1 || datosRecibidos.servoPos == 2) {
    tone(BUZZER_PIN, 1000);  // Suena el buzzer a 1000 Hz
    delay(500);               // Mantener el buzzer sonando por 500 ms
    noTone(BUZZER_PIN);       // Apagar el buzzer
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Configurar el buzzer como salida
  pinMode(BUZZER_PIN, OUTPUT);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);  // Registrar la función callback para la recepción
}

void loop() {
  // No se necesita nada en el loop ya que la recepción de datos se maneja en la callback
}
