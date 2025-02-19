#define pinGPIO9 9
#define pinGPIO20 20
#define pinGPIO19 19

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
  if (state1 || state2){
    digitalWrite(pinGPIO9, HIGH);
  } else{
    digitalWrite(pinGPIO9, LOW);
  }
}