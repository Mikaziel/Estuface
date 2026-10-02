#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>

// -------------------- PINOS --------------------
#define DHT_PIN 4 // D4
#define DHT_TYPE DHT11

#define RTC_SDA 25 // D25
#define RTC_SCL 26 // D26

// -------------------- OBJETOS --------------------
DHT dht(DHT_PIN, DHT_TYPE);
RTC_DS3231 rtc;

// -------------------- FUNÇÕES --------------------
void imprimirDoisDigitos(int numero)
{
    if (numero < 10)
    {
        Serial.print("0");
    }

    Serial.print(numero);
}

void imprimirDataHora(DateTime agora)
{
    imprimirDoisDigitos(agora.day());
    Serial.print("/");
    imprimirDoisDigitos(agora.month());
    Serial.print("/");
    Serial.print(agora.year());

    Serial.print(" ");

    imprimirDoisDigitos(agora.hour());
    Serial.print(":");
    imprimirDoisDigitos(agora.minute());
    Serial.print(":");
    imprimirDoisDigitos(agora.second());
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    dht.begin();

    Wire.begin(RTC_SDA, RTC_SCL);

    if (!rtc.begin())
    {
        Serial.println("Erro RTC");
        while (true)
        {
            delay(1000);
        }
    }

    if (rtc.lostPower())
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    Serial.println("Teste DHT11 + RTC iniciado");
}

void loop()
{
    DateTime agora = rtc.now();

    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    Serial.print("Data/Hora: ");
    imprimirDataHora(agora);

    if (isnan(temperatura) || isnan(umidade))
    {
        Serial.println(" | Erro DHT11");
    }
    else
    {
        Serial.print(" | Temp ar: ");
        Serial.print(temperatura);
        Serial.print(" C | Umidade: ");
        Serial.print(umidade);
        Serial.println(" %");
    }

    delay(2000);
}
