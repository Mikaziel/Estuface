#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pino de dados do DS18B20
#define ONE_WIRE_BUS 23

// Configuração do barramento OneWire
OneWire oneWire(ONE_WIRE_BUS);

// Objeto do sensor de temperatura
DallasTemperature sensors(&oneWire);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("Teste do sensor DS18B20 - Temperatura da agua");
    Serial.println("Iniciando...");

    // Tentativa de ajudar no teste sem resistor externo
    pinMode(ONE_WIRE_BUS, INPUT_PULLUP);

    // Inicia o sensor
    sensors.begin();

    int quantidadeSensores = sensors.getDeviceCount();

    Serial.print("Sensores DS18B20 encontrados: ");
    Serial.println(quantidadeSensores);

    if (quantidadeSensores == 0)
    {
        Serial.println("Nenhum DS18B20 encontrado.");
        Serial.println("Verifique a ligacao dos fios.");
        Serial.println("Sem resistor de 4.7k, pode ser que o sensor nao funcione.");
    }
    else
    {
        Serial.println("DS18B20 iniciado com sucesso!");
    }

    Serial.println("----------------------");
}

void loop()
{
    Serial.println("Solicitando leitura de temperatura...");

    sensors.requestTemperatures();

    float temperaturaAgua = sensors.getTempCByIndex(0);

    if (temperaturaAgua == DEVICE_DISCONNECTED_C)
    {
        Serial.println("Erro: sensor desconectado ou nao detectado.");
        Serial.println("Se estiver sem resistor, provavelmente precisa do resistor de 4.7k entre DATA e 3V3.");
    }
    else
    {
        Serial.print("Temperatura da agua: ");
        Serial.print(temperaturaAgua);
        Serial.println(" °C");
    }

    Serial.println("----------------------");

    delay(2000);
}