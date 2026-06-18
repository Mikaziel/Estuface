#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// -------------------- PINOS --------------------
#define DHT_PIN 4
#define DHT_TYPE DHT11

#define DS18B20_PIN 23

#define I2C_SDA 25
#define I2C_SCL 26

// -------------------- OBJETOS --------------------
DHT dht(DHT_PIN, DHT_TYPE);

RTC_DS3231 rtc;

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensorAgua(&oneWire);

// -------------------- FUNÇÕES AUXILIARES --------------------
void imprimirDoisDigitos(int numero)
{
    if (numero < 10)
    {
        Serial.print("0");
    }

    Serial.print(numero);
}

void imprimirDataHora()
{
    DateTime agora = rtc.now();

    Serial.print("Data: ");
    imprimirDoisDigitos(agora.day());
    Serial.print("/");
    imprimirDoisDigitos(agora.month());
    Serial.print("/");
    Serial.print(agora.year());

    Serial.print(" | Hora: ");
    imprimirDoisDigitos(agora.hour());
    Serial.print(":");
    imprimirDoisDigitos(agora.minute());
    Serial.print(":");
    imprimirDoisDigitos(agora.second());

    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("Teste integrado: RTC DS3231 + DHT11 + DS18B20");
    Serial.println("Iniciando...");

    // -------------------- RTC DS3231 --------------------
    Wire.begin(I2C_SDA, I2C_SCL);

    if (!rtc.begin())
    {
        Serial.println("ERRO: RTC DS3231 nao encontrado!");
        Serial.println("Verifique SDA, SCL, VCC e GND.");
        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("RTC DS3231 encontrado!");

    if (rtc.lostPower())
    {
        Serial.println("RTC perdeu energia ou ainda nao foi ajustado.");
        Serial.println("Ajustando data/hora para o momento da compilacao...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // -------------------- DHT11 --------------------
    dht.begin();
    Serial.println("DHT11 iniciado!");

    // -------------------- DS18B20 --------------------
    // Tentativa de usar pull-up interno, já que estamos testando sem resistor externo
    pinMode(DS18B20_PIN, INPUT_PULLUP);

    sensorAgua.begin();
    sensorAgua.setWaitForConversion(true);

    int quantidadeSensoresAgua = sensorAgua.getDeviceCount();

    Serial.print("Sensores DS18B20 encontrados: ");
    Serial.println(quantidadeSensoresAgua);

    if (quantidadeSensoresAgua == 0)
    {
        Serial.println("Aviso: DS18B20 nao encontrado no inicio.");
        Serial.println("Como esta sem resistor, pode falhar na deteccao.");
    }
    else
    {
        Serial.println("DS18B20 iniciado!");
    }

    Serial.println("Sistema iniciado.");
    Serial.println("------------------------------");
}

void loop()
{
    // -------------------- RTC --------------------
    imprimirDataHora();

    // -------------------- DHT11 --------------------
    float temperaturaAmbiente = dht.readTemperature();
    float umidadeAmbiente = dht.readHumidity();

    if (isnan(temperaturaAmbiente) || isnan(umidadeAmbiente))
    {
        Serial.println("DHT11: falha na leitura.");
    }
    else
    {
        Serial.print("Temperatura ambiente: ");
        Serial.print(temperaturaAmbiente);
        Serial.println(" °C");

        Serial.print("Umidade ambiente: ");
        Serial.print(umidadeAmbiente);
        Serial.println(" %");
    }

    // -------------------- DS18B20 --------------------
    sensorAgua.requestTemperatures();

    float temperaturaAgua = sensorAgua.getTempCByIndex(0);

    if (temperaturaAgua == DEVICE_DISCONNECTED_C)
    {
        Serial.println("DS18B20: falha na leitura.");
        Serial.println("Possivel causa: falta do resistor de 4.7k entre DATA e 3V3.");
    }
    else
    {
        Serial.print("Temperatura da agua: ");
        Serial.print(temperaturaAgua);
        Serial.println(" °C");
    }

    Serial.println("------------------------------");

    delay(2000);
}