#include <Arduino.h>

#define RELE_BOMBA 16 // P16 / GPIO16

void setup()
{
    digitalWrite(RELE_BOMBA, HIGH); // começa desligado, comum em relé ativo LOW
    pinMode(RELE_BOMBA, OUTPUT);

    Serial.begin(115200);
    Serial.println("Teste do rele da bomba no IN1");
}

void loop()
{
    Serial.println("Bomba LIGADA");
    digitalWrite(RELE_BOMBA, LOW); // liga o relé
    delay(5000);

    Serial.println("Bomba DESLIGADA");
    digitalWrite(RELE_BOMBA, HIGH); // desliga o relé
    delay(5000);
}