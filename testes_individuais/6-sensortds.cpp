#include <Arduino.h>

// Pino analogico do sensor TDS
#define TDS_PIN 34

// Referencia do ESP32
#define VREF 3.3

// Resolucao ADC do ESP32 em 12 bits: 0 a 4095
#define ADC_RESOLUTION 4095.0

// Quantidade de leituras para fazer uma media
#define NUM_AMOSTRAS 30

// Temperatura fixa para compensacao
// Depois podemos substituir pelo DS18B20
float temperaturaAgua = 25.0;

int lerMediaADC()
{
    long soma = 0;

    for (int i = 0; i < NUM_AMOSTRAS; i++)
    {
        soma += analogRead(TDS_PIN);
        delay(10);
    }

    return soma / NUM_AMOSTRAS;
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("Teste do sensor TDS / Condutividade");
    Serial.println("Iniciando...");

    // Configura o ADC do ESP32
    analogReadResolution(12);

    // Permite leitura em uma faixa mais proxima de 0 a 3.3V
    analogSetPinAttenuation(TDS_PIN, ADC_11db);

    Serial.println("Sensor TDS iniciado.");
    Serial.println("------------------------------");
}

void loop()
{
    int valorADC = lerMediaADC();

    float tensao = valorADC * (VREF / ADC_RESOLUTION);

    // Compensacao de temperatura
    // A referencia padrao e 25 graus Celsius
    float coeficienteCompensacao = 1.0 + 0.02 * (temperaturaAgua - 25.0);
    float tensaoCompensada = tensao / coeficienteCompensacao;

    // Formula aproximada comum para modulos TDS analogicos
    float tds =
        (133.42 * tensaoCompensada * tensaoCompensada * tensaoCompensada - 255.86 * tensaoCompensada * tensaoCompensada + 857.39 * tensaoCompensada) * 0.5;

    if (tds < 0)
    {
        tds = 0;
    }

    Serial.print("ADC: ");
    Serial.println(valorADC);

    Serial.print("Tensao: ");
    Serial.print(tensao, 3);
    Serial.println(" V");

    Serial.print("Temperatura usada na compensacao: ");
    Serial.print(temperaturaAgua);
    Serial.println(" °C");

    Serial.print("TDS aproximado: ");
    Serial.print(tds, 2);
    Serial.println(" ppm");

    Serial.println("------------------------------");

    delay(2000);
}