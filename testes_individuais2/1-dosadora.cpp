#include <Arduino.h>

// -------------------- PINOS --------------------
#define LED_INTERNO 2    // D2
#define RELE_DOSADORA 18 // D18 -> IN3 do relé

// -------------------- RELÉ --------------------
#define RELE_LIGADO LOW
#define RELE_DESLIGADO HIGH

// -------------------- TEMPOS --------------------
#define TEMPO_DOSADORA_LIGADA_MS 7000UL
#define TEMPO_DOSADORA_DESLIGADA_MS (8UL * 60UL * 60UL * 1000UL)
#define TEMPO_CICLO_DOSADORA_MS (TEMPO_DOSADORA_LIGADA_MS + TEMPO_DOSADORA_DESLIGADA_MS)

// -------------------- VARIÁVEIS --------------------
bool dosadoraLigada = false;
unsigned long inicioCicloDosadora = 0;

// -------------------- FUNÇÕES --------------------
void ligarDosadora()
{
    digitalWrite(RELE_DOSADORA, RELE_LIGADO);
    digitalWrite(LED_INTERNO, HIGH);

    dosadoraLigada = true;
    Serial.println("DOSADORA LIGADA");
}

void desligarDosadora()
{
    digitalWrite(RELE_DOSADORA, RELE_DESLIGADO);
    digitalWrite(LED_INTERNO, LOW);

    dosadoraLigada = false;
    Serial.println("DOSADORA DESLIGADA");
}

void controlarDosadora()
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
        }
    }
    else
    {
        if (dosadoraLigada)
        {
            desligarDosadora();
        }
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_INTERNO, OUTPUT);
    pinMode(RELE_DOSADORA, OUTPUT);

    digitalWrite(LED_INTERNO, LOW);
    digitalWrite(RELE_DOSADORA, RELE_DESLIGADO);

    inicioCicloDosadora = millis();

    Serial.println("Teste dosadora iniciado");

    // Inicia o primeiro ciclo imediatamente: 7 segundos ligada e depois 8 horas desligada.
    controlarDosadora();
}

void loop()
{
    controlarDosadora();
    delay(50);
}
