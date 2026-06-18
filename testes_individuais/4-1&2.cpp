#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>

#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
RTC_DS3231 rtc;

unsigned long tempoAnteriorRTC = 0;
unsigned long tempoAnteriorDHT = 0;

float temperatura = 0;
float umidade = 0;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("Teste do RTC DS3231 e do sensor DHT11");
    Serial.println("Iniciando...");

    Wire.begin(25, 26); // SDA = GPIO 25, SCL = GPIO 26

    if (!rtc.begin())
    {
        Serial.println("RTC DS3231 nao encontrado!");
        while (1)
        {
            delay(1000);
        }
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC perdeu energia. Ajustando data/hora...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    dht.begin();

    Serial.println("RTC e DHT11 iniciados!");
    Serial.println("----------------------");
}

void loop()
{
    unsigned long tempoAtual = millis();

    // Lê o DHT11 a cada 2 segundos
    if (tempoAtual - tempoAnteriorDHT >= 2000)
    {
        tempoAnteriorDHT = tempoAtual;

        float novaUmidade = dht.readHumidity();
        float novaTemperatura = dht.readTemperature();

        if (!isnan(novaUmidade) && !isnan(novaTemperatura))
        {
            umidade = novaUmidade;
            temperatura = novaTemperatura;
        }
        else
        {
            Serial.println("Falha ao ler o sensor DHT11!");
        }
    }

    // Mostra o RTC a cada 1 segundo
    if (tempoAtual - tempoAnteriorRTC >= 1000)
    {
        tempoAnteriorRTC = tempoAtual;

        DateTime agora = rtc.now();

        Serial.print("Data: ");
        Serial.print(agora.day());
        Serial.print("/");
        Serial.print(agora.month());
        Serial.print("/");
        Serial.print(agora.year());

        Serial.print(" | Hora: ");
        Serial.print(agora.hour());
        Serial.print(":");
        Serial.print(agora.minute());
        Serial.print(":");
        Serial.println(agora.second());

        Serial.print("Temperatura ambiente: ");
        Serial.print(temperatura);
        Serial.println(" °C");

        Serial.print("Umidade ambiente: ");
        Serial.print(umidade);
        Serial.println(" %");

        Serial.println("----------------------");
    }
}