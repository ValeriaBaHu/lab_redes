int boton=15;
int led=13;


int status_boton=0;
void setup() {
 pinMode (led, OUTPUT);
 pinMode (boton, INPUT);
}

void loop() {
  status_boton=digitalRead(boton);
  if(status_boton==1){
    digitalWrite(led,HIGH);

  }
  else{ 
    digitalWrite(led, LOW);
  }
}