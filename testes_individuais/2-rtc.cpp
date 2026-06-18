#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("Teste do RTC DS3231");
    Serial.println("Iniciando...");

    // Inicia a comunicação I2C nos pinos padrão do ESP32
    Wire.begin(25, 26); // SDA = GPIO 25, SCL = GPIO 26

    if (!rtc.begin())
    {
        Serial.println("RTC DS3231 nao encontrado!");
        Serial.println("Verifique os fios SDA, SCL, VCC e GND.");
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

    Serial.println("RTC iniciado com sucesso!");
}

void loop()
{
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

    delay(1000);
}