#include <Arduino.h>

#define RELE_BOMBA 16  // P16 / GPIO16 -> IN1 do relé
#define SENSOR_BOIA 27 // GPIO27 -> sensor boia

#define RELE_LIGADO LOW
#define RELE_DESLIGADO HIGH

#define NIVEL_OK HIGH
#define NIVEL_BAIXO LOW

void setup()
{
    Serial.begin(115200);
    delay(1000);

    digitalWrite(RELE_BOMBA, RELE_DESLIGADO);
    pinMode(RELE_BOMBA, OUTPUT);

    pinMode(SENSOR_BOIA, INPUT_PULLUP);

    Serial.println("Teste: bomba só funciona com nível de água OK");
    Serial.println("--------------------------------");
}

void loop()
{
    int estadoBoia = digitalRead(SENSOR_BOIA);

    Serial.print("Estado da boia: ");
    Serial.print(estadoBoia);

    if (estadoBoia == NIVEL_OK)
    {
        Serial.println(" | NIVEL OK -> Bomba LIGADA");
        digitalWrite(RELE_BOMBA, RELE_LIGADO);
    }
    else
    {
        Serial.println(" | NIVEL BAIXO -> Bomba DESLIGADA");
        digitalWrite(RELE_BOMBA, RELE_DESLIGADO);
    }

    delay(500);
}