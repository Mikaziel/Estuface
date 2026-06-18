#include <Arduino.h>

#define RELE_BOMBA 16     // P16 / GPIO16
#define RELE_VENTOINHA 17 // P17 / GPIO17

void setup()
{
    digitalWrite(RELE_BOMBA, HIGH);     // começa desligado, comum em relé ativo LOW
    digitalWrite(RELE_VENTOINHA, HIGH); // começa desligado, comum em relé ativo LOW
    pinMode(RELE_BOMBA, OUTPUT);
    pinMode(RELE_VENTOINHA, OUTPUT);

    Serial.begin(115200);
    Serial.println("Teste do rele da bomba no IN1");
    Serial.println("Teste do rele da ventoinha no IN2");
}

void loop()
{
    Serial.println("Bomba LIGADA");
    digitalWrite(RELE_BOMBA, LOW);      // liga o relé
    digitalWrite(RELE_VENTOINHA, HIGH); // liga a ventoinha
    delay(5000);

    Serial.println("Bomba DESLIGADA");
    digitalWrite(RELE_BOMBA, HIGH);    // desliga o relé
    digitalWrite(RELE_VENTOINHA, LOW); // desliga a ventoinha
    delay(5000);
}