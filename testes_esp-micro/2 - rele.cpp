#include <Arduino.h>

#define RELE_IN1 16 // RX2 na placa ESP32 micro USB

#define RELE_LIGADO LOW
#define RELE_DESLIGADO HIGH

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(RELE_IN1, OUTPUT);
    digitalWrite(RELE_IN1, RELE_DESLIGADO);

    Serial.println();
    Serial.println("Teste do rele IN1");
    Serial.println("Pino usado: GPIO16 / RX2");
    Serial.println("Relé inicia desligado.");
}

void loop()
{
    Serial.println("IN1 LIGADO");
    digitalWrite(RELE_IN1, RELE_LIGADO);
    delay(10000);

    Serial.println("IN1 DESLIGADO");
    digitalWrite(RELE_IN1, RELE_DESLIGADO);
    delay(10000);
}