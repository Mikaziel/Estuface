#include <Arduino.h>

// -------------------- PINOS --------------------
#define BOIA_AVISO 27   // D27
#define BOIA_CRITICA 14 // D14
#define LED_BOMBA 2     // D2 - simula a bomba principal

// -------------------- LÓGICA DAS BOIAS --------------------
// Boia aviso:
// 0 = nível abaixando
// 1 = nível OK
#define AVISO_NIVEL_BAIXO LOW
#define AVISO_NIVEL_OK HIGH

// Boia crítica:
// 0 = nível seguro
// 1 = nível crítico
#define CRITICA_NIVEL_OK LOW
#define CRITICA_NIVEL_CRITICO HIGH

// -------------------- VARIÁVEIS --------------------
int ultimoAviso = -1;
int ultimaCritica = -1;
bool ultimaBombaLigada = false;
bool primeiroPrint = true;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(BOIA_AVISO, INPUT_PULLUP);
    pinMode(BOIA_CRITICA, INPUT_PULLUP);
    pinMode(LED_BOMBA, OUTPUT);

    digitalWrite(LED_BOMBA, LOW);

    Serial.println("Teste boias + simulacao da bomba iniciado");
}

void loop()
{
    int aviso = digitalRead(BOIA_AVISO);
    int critica = digitalRead(BOIA_CRITICA);

    bool bombaLigada;

    if (critica == CRITICA_NIVEL_CRITICO)
    {
        bombaLigada = false;
    }
    else
    {
        bombaLigada = true;
    }

    digitalWrite(LED_BOMBA, bombaLigada ? HIGH : LOW);

    if (primeiroPrint || aviso != ultimoAviso || critica != ultimaCritica || bombaLigada != ultimaBombaLigada)
    {
        primeiroPrint = false;
        ultimoAviso = aviso;
        ultimaCritica = critica;
        ultimaBombaLigada = bombaLigada;

        Serial.print("Aviso: ");
        Serial.print(aviso);

        Serial.print(" | Critica: ");
        Serial.print(critica);

        Serial.print(" | ");

        if (!bombaLigada)
        {
            Serial.println("BOMBA DESLIGADA - NIVEL CRITICO");
        }
        else if (aviso == AVISO_NIVEL_BAIXO)
        {
            Serial.println("BOMBA FUNCIONANDO - AVISO: NIVEL ABAIXANDO");
        }
        else
        {
            Serial.println("BOMBA FUNCIONANDO - NIVEL OK");
        }
    }

    delay(100);
}
