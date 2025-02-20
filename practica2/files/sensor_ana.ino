#include <DHT.h>
int led=32;
DHT dht (33, DHT22);

int valor_temperatura=0;
int valor_humedad=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  dht.begin();
  Serial.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  valor_temperatura=dht.readTemperature();
  valor_humedad=dht.readHumidity();
  Serial.print("Temperatura:");
  Serial.println(valor_temperatura);
  Serial.print("Humedad:");
  Serial.println(valor_humedad);

  if(valor_humedad>=50){
    digitalWrite(led,HIGH);

  }
  else{
    digitalWrite(led, LOW);

  }

}