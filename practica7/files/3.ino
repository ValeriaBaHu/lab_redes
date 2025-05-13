int led=25;
int boton=26;
 
int status_boton=0;
 
void setup(){
  pinMode(led, OUTPUT);
  pinMode(boton, INPUT);
}
 
void loop(){
  status_boton=digitalRead(boton);
  digitalWrite(led, status_boton);
}