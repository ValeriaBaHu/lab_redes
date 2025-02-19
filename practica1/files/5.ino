#define pinGPIO9 9   // GPIO adecuado para salida en ESP32-C6
#define pinGPIO20 20   // Entrada 1
#define pinGPIO19 19   // Entrada 2

int state1 = 0;
int state2 = 0;

void setup() {
  pinMode(pinGPIO9, OUTPUT);
  pinMode(pinGPIO20, INPUT);
  pinMode(pinGPIO19, INPUT);
  digitalWrite(pinGPIO9, LOW);
}

void loop() {
  state1 = digitalRead(pinGPIO20);
  state2 = digitalRead(pinGPIO19);
  if (state1 && state2){
    digitalWrite(pinGPIO9, HIGH);
  } else{
    digitalWrite(pinGPIO9, LOW);
  }
}