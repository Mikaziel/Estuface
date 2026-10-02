#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// -------------------- PINOS --------------------
#define DS18B20_PIN 23 // D23

// -------------------- OBJETOS --------------------
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensorAgua(&oneWire);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    sensorAgua.begin();
    sensorAgua.setWaitForConversion(true);

    Serial.println("Teste DS18B20 iniciado");
}

void loop()
{
    sensorAgua.requestTemperatures();

    float temperaturaAgua = sensorAgua.getTempCByIndex(0);

    if (temperaturaAgua == DEVICE_DISCONNECTED_C || temperaturaAgua == -127.00)
    {
        Serial.println("Erro DS18B20");
    }
    else
    {
        Serial.print("Temp agua: ");
        Serial.print(temperaturaAgua);
        Serial.println(" C");
    }

    delay(2000);
}
