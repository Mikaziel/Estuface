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

#define TDS_PIN 34

#define RTC_SDA 25
#define RTC_SCL 26

#define RELE_BOMBA 16     // P16 / GPIO16 -> IN1
#define RELE_VENTOINHA 17 // P17 / GPIO17 -> IN2

// -------------------- CONFIGURAÇÃO DOS RELÉS --------------------
#define RELE_LIGADO LOW
#define RELE_DESLIGADO HIGH

#define BOMBA_PRINCIPAL_SEMPRE_LIGADA true

#define TEMP_LIGAR_VENTOINHA 30.0
#define TEMP_DESLIGAR_VENTOINHA 28.0

// -------------------- TDS --------------------
#define VREF 3.3
#define ADC_RESOLUTION 4095.0
#define NUM_AMOSTRAS_TDS 30

// -------------------- OBJETOS --------------------
DHT dht(DHT_PIN, DHT_TYPE);

RTC_DS3231 rtc;

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensorAgua(&oneWire);

// -------------------- VARIÁVEIS DOS ATUADORES --------------------
bool bombaLigada = false;
bool ventoinhaLigada = false;

// -------------------- FUNÇÕES --------------------
void imprimirDoisDigitos(int numero)
{
    if (numero < 10)
    {
        Serial.print("0");
    }

    Serial.print(numero);
}

void ligarBomba()
{
    digitalWrite(RELE_BOMBA, RELE_LIGADO);
    bombaLigada = true;
}

void desligarBomba()
{
    digitalWrite(RELE_BOMBA, RELE_DESLIGADO);
    bombaLigada = false;
}

void ligarVentoinha()
{
    digitalWrite(RELE_VENTOINHA, RELE_LIGADO);
    ventoinhaLigada = true;
}

void desligarVentoinha()
{
    digitalWrite(RELE_VENTOINHA, RELE_DESLIGADO);
    ventoinhaLigada = false;
}

int lerMediaTDS()
{
    long soma = 0;

    for (int i = 0; i < NUM_AMOSTRAS_TDS; i++)
    {
        soma += analogRead(TDS_PIN);
        delay(10);
    }

    return soma / NUM_AMOSTRAS_TDS;
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

float calcularTDS(float tensao, float temperaturaAgua)
{
    float coeficienteCompensacao = 1.0 + 0.02 * (temperaturaAgua - 25.0);
    float tensaoCompensada = tensao / coeficienteCompensacao;

    float tds =
        (133.42 * tensaoCompensada * tensaoCompensada * tensaoCompensada -
         255.86 * tensaoCompensada * tensaoCompensada +
         857.39 * tensaoCompensada) *
        0.5;

    if (tds < 0)
    {
        tds = 0;
    }

    return tds;
}

void controlarAtuadores(float temperaturaAmbiente, bool dhtOk)
{
    // -------------------- BOMBA PRINCIPAL --------------------
    if (BOMBA_PRINCIPAL_SEMPRE_LIGADA)
    {
        if (!bombaLigada)
        {
            ligarBomba();
            Serial.println("Bomba principal: LIGADA");
        }
    }
    else
    {
        if (bombaLigada)
        {
            desligarBomba();
            Serial.println("Bomba principal: DESLIGADA");
        }
    }

    // -------------------- VENTOINHAS --------------------
    if (!dhtOk)
    {
        Serial.println("Ventoinhas: controle ignorado por falha no DHT11.");
        return;
    }

    if (temperaturaAmbiente >= TEMP_LIGAR_VENTOINHA && !ventoinhaLigada)
    {
        ligarVentoinha();
        Serial.println("Ventoinhas: LIGADAS");
    }
    else if (temperaturaAmbiente <= TEMP_DESLIGAR_VENTOINHA && ventoinhaLigada)
    {
        desligarVentoinha();
        Serial.println("Ventoinhas: DESLIGADAS");
    }

    Serial.print("Estado bomba: ");
    Serial.println(bombaLigada ? "LIGADA" : "DESLIGADA");

    Serial.print("Estado ventoinhas: ");
    Serial.println(ventoinhaLigada ? "LIGADAS" : "DESLIGADAS");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("Teste integrado: RTC + DHT11 + DS18B20 + TDS + Rele + Bomba + Ventoinhas");
    Serial.println("Iniciando...");

    // -------------------- RELÉS --------------------
    digitalWrite(RELE_BOMBA, RELE_DESLIGADO);
    digitalWrite(RELE_VENTOINHA, RELE_DESLIGADO);

    pinMode(RELE_BOMBA, OUTPUT);
    pinMode(RELE_VENTOINHA, OUTPUT);

    Serial.println("Reles iniciados desligados!");

    // -------------------- RTC DS3231 --------------------
    Wire.begin(RTC_SDA, RTC_SCL);

    if (!rtc.begin())
    {
        Serial.println("ERRO: RTC DS3231 nao encontrado!");
        Serial.println("Verifique SDA no P25, SCL no P26, VCC e GND.");

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
    pinMode(DS18B20_PIN, INPUT_PULLUP);
    sensorAgua.begin();

    int quantidadeDS18B20 = sensorAgua.getDeviceCount();

    Serial.print("Sensores DS18B20 encontrados: ");
    Serial.println(quantidadeDS18B20);

    if (quantidadeDS18B20 == 0)
    {
        Serial.println("Aviso: DS18B20 nao encontrado no inicio.");
        Serial.println("Se estiver sem resistor, pode falhar na leitura.");
    }
    else
    {
        Serial.println("DS18B20 iniciado!");
    }

    // -------------------- TDS --------------------
    analogReadResolution(12);
    analogSetPinAttenuation(TDS_PIN, ADC_11db);

    Serial.println("TDS iniciado no P34/GPIO34!");

    Serial.println("Sistema iniciado com sucesso!");
    Serial.println("--------------------------------");
}

void loop()
{
    // -------------------- RTC --------------------
    imprimirDataHora();

    // -------------------- DHT11 --------------------
    float temperaturaAmbiente = dht.readTemperature();
    float umidadeAmbiente = dht.readHumidity();

    bool dhtOk = !(isnan(temperaturaAmbiente) || isnan(umidadeAmbiente));

    if (!dhtOk)
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
        Serial.println("Usando 25 °C como temperatura padrao para compensar o TDS.");
        temperaturaAgua = 25.0;
    }
    else
    {
        Serial.print("Temperatura da agua: ");
        Serial.print(temperaturaAgua);
        Serial.println(" °C");
    }

    // -------------------- TDS --------------------
    int adcTDS = lerMediaTDS();

    float tensaoTDS = adcTDS * (VREF / ADC_RESOLUTION);

    float tdsPPM = calcularTDS(tensaoTDS, temperaturaAgua);

    Serial.print("TDS ADC: ");
    Serial.println(adcTDS);

    Serial.print("TDS Tensao: ");
    Serial.print(tensaoTDS, 3);
    Serial.println(" V");

    Serial.print("TDS aproximado: ");
    Serial.print(tdsPPM, 2);
    Serial.println(" ppm");

    // -------------------- ATUADORES --------------------
    controlarAtuadores(temperaturaAmbiente, dhtOk);

    Serial.println("--------------------------------");

    delay(2000);
}