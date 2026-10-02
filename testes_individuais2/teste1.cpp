#include <Arduino.h>

#define LED_INTERNO 2

unsigned long contador = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_INTERNO, OUTPUT);

  Serial.println();
  Serial.println("================================");
  Serial.println("Teste 1 - ESP32 DevKit V1");
  Serial.println("Upload e Monitor Serial funcionando");
  Serial.println("Ambiente: esp32-devkit-v1");
  Serial.println("================================");
}

void loop()
{
  contador++;

  Serial.print("ESP32 DevKit V1 funcionando | Ciclo: ");
  Serial.println(contador);

  digitalWrite(LED_INTERNO, HIGH);
  delay(500);

  digitalWrite(LED_INTERNO, LOW);
  delay(500);
}