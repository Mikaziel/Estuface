#include <Arduino.h>

// -------------------- PINOS --------------------
#define BOIA_CRITICA 14 // D14

// -------------------- LÓGICA --------------------
// 0 = nível seguro
// 1 = nível crítico
#define CRITICA_NIVEL_OK LOW
#define CRITICA_NIVEL_CRITICO HIGH

// -------------------- VARIÁVEIS --------------------
int estadoAnterior = -1;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(BOIA_CRITICA, INPUT_PULLUP);

    Serial.println("Teste boia critica iniciado");
}

void loop()
{
    int critica = digitalRead(BOIA_CRITICA);

    if (critica != estadoAnterior)
    {
        estadoAnterior = critica;

        Serial.print("Critica: ");
        Serial.print(critica);
        Serial.print(" | ");

        if (critica == CRITICA_NIVEL_CRITICO)
        {
            Serial.println("NIVEL CRITICO");
        }
        else
        {
            Serial.println("NIVEL SEGURO");
        }
    }

    delay(100);
}
