#include <Arduino.h>
#include <DHT.h>

// Pino de dados do DHT11
#define DHT_PIN 4

// Tipo do sensor
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("Teste do sensor DHT11");
    Serial.println("Iniciando...");

    dht.begin();

    Serial.println("DHT11 iniciado!");
}

void loop()
{
    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (isnan(umidade) || isnan(temperatura))
    {
        Serial.println("Falha ao ler o sensor DHT11!");
        delay(2000);
        return;
    }

    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");

    Serial.println("----------------------");

    delay(2000);
}