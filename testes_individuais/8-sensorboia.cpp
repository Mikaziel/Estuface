#include <Arduino.h>

#define SENSOR_BOIA 27 // Pode trocar para outro GPIO livre

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(SENSOR_BOIA, INPUT_PULLUP);

    Serial.println("Teste individual do sensor boia");
    Serial.println("Mexa a boia para ver a mudanca no monitor serial.");
    Serial.println("--------------------------------");
}

void loop()
{
    int estado = digitalRead(SENSOR_BOIA);

    Serial.print("Leitura bruta: ");
    Serial.print(estado);

    if (estado == LOW)
    {
        Serial.println(" | SENSOR FECHADO / NIVEL DETECTADO");
    }
    else
    {
        Serial.println(" | SENSOR ABERTO / NIVEL NAO DETECTADO");
    }

    delay(500);
}