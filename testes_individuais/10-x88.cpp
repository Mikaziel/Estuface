#include <Arduino.h>

#define RELE_BOMBA 16   // P16 / GPIO16 -> IN1 do relé
#define BOIA_AVISO 27   // GPIO27 -> boia de aviso/intermediária
#define BOIA_CRITICA 14 // GPIO14 -> boia vertical/crítica

#define RELE_LIGADO LOW
#define RELE_DESLIGADO HIGH

// -------------------- LÓGICA INDIVIDUAL DAS BOIAS --------------------
// Boia de aviso:
#define AVISO_NIVEL_OK HIGH
#define AVISO_NIVEL_BAIXO LOW

// Boia crítica está invertida:
#define CRITICO_NIVEL_OK LOW
#define CRITICO_NIVEL_BAIXO HIGH

void setup()
{
    Serial.begin(115200);
    delay(1000);

    digitalWrite(RELE_BOMBA, RELE_DESLIGADO);
    pinMode(RELE_BOMBA, OUTPUT);

    pinMode(BOIA_AVISO, INPUT_PULLUP);
    pinMode(BOIA_CRITICA, INPUT_PULLUP);

    Serial.println("Teste: bomba com boia de aviso + boia critica");
    Serial.println("--------------------------------");
}

void loop()
{
    int estadoBoiaAviso = digitalRead(BOIA_AVISO);
    int estadoBoiaCritica = digitalRead(BOIA_CRITICA);

    bool avisoNivelOk = estadoBoiaAviso == HIGH;
    bool criticoNivelOk = estadoBoiaCritica == LOW; // invertida

    Serial.print("Boia aviso: ");
    Serial.print(avisoNivelOk ? 1 : 0);

    Serial.print(" | Boia critica: ");
    Serial.println(criticoNivelOk ? 1 : 0);

    if (!criticoNivelOk)
    {
        digitalWrite(RELE_BOMBA, RELE_DESLIGADO);

        Serial.println("ALERTA CRITICO: nivel muito baixo!");
        Serial.println("Volume estimado: aproximadamente 33%");
        Serial.println("Bomba DESLIGADA para evitar funcionamento seco.");
    }
    else
    {
        digitalWrite(RELE_BOMBA, RELE_LIGADO);

        if (!avisoNivelOk)
        {
            Serial.println("AVISO: nivel abaixo do ideal.");
            Serial.println("Volume estimado: aproximadamente 86%");
            Serial.println("Bomba continua LIGADA.");
        }
        else
        {
            Serial.println("Nivel normal.");
            Serial.println("Volume estimado: 100%");
            Serial.println("Bomba LIGADA.");
        }
    }

    Serial.println("--------------------------------");
    delay(1000);
}