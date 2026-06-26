#include <Arduino.h>

#define PH_PIN 34

#define VREF 3.3
#define ADC_RESOLUTION 4095.0

// Referência provisória baseada na água do campus
float TENSAO_REFERENCIA_AGUA = 1.350;
float PH_REFERENCIA_AGUA = 7.00;

// Valor aproximado. Depois será substituído por calibração real.
float VOLTS_POR_PH = 0.18;

float calcularPHProvisorio(float tensao)
{
  float ph = PH_REFERENCIA_AGUA - ((tensao - TENSAO_REFERENCIA_AGUA) / VOLTS_POR_PH);
  return ph;
}

int lerMediaADC(int quantidadeAmostras)
{
  long soma = 0;

  for (int i = 0; i < quantidadeAmostras; i++)
  {
    soma += analogRead(PH_PIN);
    delay(10);
  }

  return soma / quantidadeAmostras;
}

void fazerLeituraEstabilizada()
{
  Serial.println();
  Serial.println("Coloque a sonda no liquido.");
  Serial.println("Aguardando estabilizacao por 60 segundos...");
  Serial.println("--------------------------------");

  float somaPH = 0;
  float somaTensao = 0;
  int quantidadeLeituras = 0;

  for (int segundo = 1; segundo <= 60; segundo++)
  {
    int adc = lerMediaADC(30);
    float tensao = adc * (VREF / ADC_RESOLUTION);
    float ph = calcularPHProvisorio(tensao);

    // Usa somente os ultimos 30 segundos para a media final
    if (segundo > 30)
    {
      somaPH += ph;
      somaTensao += tensao;
      quantidadeLeituras++;
    }

    Serial.print("Tempo: ");
    Serial.print(segundo);
    Serial.print("s | ADC: ");
    Serial.print(adc);
    Serial.print(" | Tensao: ");
    Serial.print(tensao, 3);
    Serial.print(" V | pH provisório: ");
    Serial.println(ph, 2);

    delay(700);
  }

  float phFinal = somaPH / quantidadeLeituras;
  float tensaoFinal = somaTensao / quantidadeLeituras;

  Serial.println("--------------------------------");
  Serial.println("RESULTADO ESTABILIZADO");
  Serial.print("Tensao media final: ");
  Serial.print(tensaoFinal, 3);
  Serial.println(" V");

  Serial.print("pH estimado PROVISORIO final: ");
  Serial.println(phFinal, 2);
  Serial.println("--------------------------------");
  Serial.println("Para testar outro liquido, enxague a sonda e aperte RESET no ESP32.");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  analogReadResolution(12);
  analogSetPinAttenuation(PH_PIN, ADC_11db);

  Serial.println();
  Serial.println("Sensor de pH - leitura estabilizada");
  Serial.println("ATENCAO: pH ainda provisório, sem calibracao real.");
  Serial.println("--------------------------------");

  fazerLeituraEstabilizada();
}

void loop()
{
  // Nada aqui. Para novo teste, aperte RESET no ESP32.
}