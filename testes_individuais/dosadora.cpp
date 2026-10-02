#include <Arduino.h>

// -------------------- PINO DO RELÉ DA DOSADORA --------------------
#define RELE_DOSADORA 18 // P18 / GPIO18 -> IN3 do relé

// -------------------- CONFIGURAÇÃO DO RELÉ --------------------
// A maioria dos módulos relé é ativo em LOW
#define RELE_LIGADO LOW
#define RELE_DESLIGADO HIGH

// timer dosadora 8seg ligada / 8h desligada
#define TEMPO_DOSADORA_LIGADA_MS 8000UL
#define TEMPO_DOSADORA_DESLIGADA_MS (8UL * 60UL * 60UL * 1000UL)
#define TEMPO_CICLO_DOSADORA_MS (TEMPO_DOSADORA_LIGADA_MS + TEMPO_DOSADORA_DESLIGADA_MS)

// variaveis
bool dosadoraLigada = false;

unsigned long inicioCicloDosadora = 0;

// funcoes da dosadora
void ligarDosadora()
{
    digitalWrite(RELE_DOSADORA, RELE_LIGADO);
    dosadoraLigada = true;
}

void desligarDosadora()
{
    digitalWrite(RELE_DOSADORA, RELE_DESLIGADO);
    dosadoraLigada = false;
}

void controlarDosadoraPorTimer()
{
    unsigned long agora = millis();
    unsigned long tempoNoCiclo = agora - inicioCicloDosadora;

    if (tempoNoCiclo >= TEMPO_CICLO_DOSADORA_MS)
    {
        inicioCicloDosadora = agora;
        tempoNoCiclo = 0;
    }

    if (tempoNoCiclo < TEMPO_DOSADORA_LIGADA_MS)
    {
        if (!dosadoraLigada)
        {
            ligarDosadora();
            Serial.println("Dosadora: LIGADA por 7 segundos.");
        }
    }
    else
    {
        if (dosadoraLigada)
        {
            desligarDosadora();
            Serial.println("Dosadora: DESLIGADA por 8 horas.");
        }
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    digitalWrite(RELE_DOSADORA, RELE_DESLIGADO);
    pinMode(RELE_DOSADORA, OUTPUT);

    dosadoraLigada = false;
    inicioCicloDosadora = millis();

    controlarDosadoraPorTimer();
    Serial.println("Sistema da dosadora iniciado.");
}

void loop()
{
    controlarDosadoraPorTimer();

    static unsigned long ultimoStatus = 0;

    if (millis() - ultimoStatus >= 5000UL)
    {
        ultimoStatus = millis();

        Serial.print("Estado da dosadora: ");
        Serial.println(dosadoraLigada ? "LIGADA" : "DESLIGADA");
    }

    delay(50);
}