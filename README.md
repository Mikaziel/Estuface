# Estuface - ESP32

Projeto de automação para cultivo hidropônico NFT utilizando ESP32, sensores ambientais, sensores de água, boias de nível e atuadores controlados por relé.

O objetivo do sistema é monitorar variáveis importantes do cultivo, como temperatura e umidade do ambiente, temperatura da água e condutividade/TDS da solução, além de controlar bomba principal, ventoinhas e uma bomba dosadora peristáltica por temporizador.

## Plataforma utilizada

- VS Code
- PlatformIO
- ESP32 NodeMCU-32S / ESP32 de 38 pinos
- Framework Arduino
- Monitor Serial em 115200 baud

## Estrutura do projeto

```txt
Estuface
├── include
├── lib
├── platformio.ini
├── src
│   └── main.cpp
├── test
└── testes_individuais
```

A pasta `src` contém o código principal que será compilado e enviado para o ESP32.

A pasta `testes_individuais` armazena códigos separados utilizados durante os testes individuais de sensores, módulos e atuadores.

## Sensores e módulos trabalhados

| Sensor/Módulo  | Função                                 | Status   |
| -------------- | -------------------------------------- | -------- |
| DHT11          | Mede temperatura e umidade do ambiente | Testado  |
| RTC DS3231     | Mantém data e hora em tempo real       | Testado  |
| DS18B20        | Mede a temperatura da água             | Testado  |
| TDS/EC         | Mede condutividade/TDS da água         | Testado  |
| Sensor de boia | Monitora o nível da água               | Testado  |
| Sensor de pH   | Mede o pH da água/solução nutritiva    | Pendente |

## Pinos dos sensores

| Sensor/Módulo | Pino do sensor | Pino no ESP32 |
| ------------- | -------------- | ------------- |
| DHT11         | DATA           | GPIO4         |
| RTC DS3231    | SDA            | GPIO25        |
| RTC DS3231    | SCL            | GPIO26        |
| DS18B20       | DATA           | GPIO23        |
| TDS/EC        | A / Analógico  | GPIO34        |
| Boia de aviso | Sinal          | GPIO27        |
| Boia crítica  | Sinal          | GPIO14        |
| Sensor de pH  | A / Analógico  | GPIO35        |

## Pinos dos atuadores

| Atuador               | Pino no ESP32 | Canal do relé |
| --------------------- | ------------- | ------------- |
| Bomba principal       | GPIO16        | IN1           |
| Ventoinhas            | GPIO17        | IN2           |
| Dosadora peristáltica | GPIO18        | IN3           |

Inicialmente estavam previstos pinos para três bombas dosadoras peristálticas, porém no estágio atual do projeto apenas uma dosadora está sendo utilizada.

A dosadora atual funciona como um temporizador para dosagem de água em uma mini estufa de germinação.

## Ligações principais

### DHT11

| Pino do DHT11 | Ligação |
| ------------- | ------- |
| VCC           | 3V3     |
| GND           | GND     |
| DATA          | GPIO4   |

### RTC DS3231

| Pino do RTC | Ligação |
| ----------- | ------- |
| VCC         | 3V3     |
| GND         | GND     |
| SDA         | GPIO25  |
| SCL         | GPIO26  |

O RTC foi movido dos pinos padrão `GPIO21/GPIO22` para `GPIO25/GPIO26`, liberando o `GPIO21` para uso futuro em atuadores.

### DS18B20

| Fio do DS18B20 | Ligação |
| -------------- | ------- |
| Vermelho       | 3V3     |
| Preto          | GND     |
| Amarelo        | GPIO23  |

O DS18B20 utiliza um resistor pull-up entre o fio de dados e o 3V3.

Valor ideal:

```txt
4.7kΩ
```

Como alternativa, pode ser utilizado um conjunto de resistores de `1kΩ` em série, formando aproximadamente `5kΩ`.

Ligação do resistor:

```txt
3V3 ---- resistor ---- DATA
```

Ou seja:

```txt
Fio vermelho / 3V3 ---- resistor ---- fio amarelo / DATA / GPIO23
```

### TDS/EC

| Pino do módulo TDS | Ligação |
| ------------------ | ------- |
| +                  | 3V3     |
| -                  | GND     |
| A                  | GPIO34  |

O sensor TDS/EC é analógico e utiliza o pino `GPIO34`, que é adequado para leitura analógica no ESP32.

### Boias

| Boia          | Pino   | Função                       |
| ------------- | ------ | ---------------------------- |
| Boia de aviso | GPIO27 | Indica nível abaixo do ideal |
| Boia crítica  | GPIO14 | Indica nível crítico         |

A boia de aviso segue a lógica:

```cpp
HIGH = nível OK
LOW  = nível abaixo do ideal
```

A boia crítica está invertida:

```cpp
LOW  = nível OK
HIGH = nível crítico
```

Quando o nível crítico é detectado, a bomba principal é desligada para evitar funcionamento seco.

## Bibliotecas utilizadas

As principais bibliotecas utilizadas até o momento são:

```ini
adafruit/RTClib
adafruit/Adafruit BusIO
adafruit/DHT sensor library
adafruit/Adafruit Unified Sensor
paulstoffregen/OneWire
milesburton/DallasTemperature
```

## Função das bibliotecas

| Biblioteca                | Função                                            |
| ------------------------- | ------------------------------------------------- |
| `RTClib`                  | Comunicação e leitura do RTC DS3231               |
| `Adafruit BusIO`          | Dependência utilizada por bibliotecas da Adafruit |
| `DHT sensor library`      | Leitura do sensor DHT11                           |
| `Adafruit Unified Sensor` | Dependência utilizada pela biblioteca do DHT      |
| `OneWire`                 | Comunicação OneWire utilizada pelo DS18B20        |
| `DallasTemperature`       | Leitura de temperatura do DS18B20                 |

## Configuração do `platformio.ini`

Exemplo de configuração utilizada no projeto:

```ini
[env:nodemcu-32s]
platform = espressif32
board = nodemcu-32s
framework = arduino
monitor_speed = 115200

lib_deps =
    adafruit/RTClib
    adafruit/Adafruit BusIO
    adafruit/DHT sensor library
    adafruit/Adafruit Unified Sensor
    paulstoffregen/OneWire
    milesburton/DallasTemperature
```

## Lógica dos relés

O projeto utiliza um módulo relé para controlar os atuadores.

No código atual, os relés são configurados como ativos em nível baixo:

```cpp
#define RELE_LIGADO LOW
#define RELE_DESLIGADO HIGH
```

Isso significa que:

```txt
LOW  = relé ligado
HIGH = relé desligado
```

Antes de configurar os pinos como saída, o código define os relés como desligados para evitar acionamentos indesejados ao iniciar o ESP32.

## Bomba principal

A bomba principal é controlada pelas boias de nível.

Pino utilizado:

```cpp
#define RELE_BOMBA 16
```

A lógica é:

- Se a boia crítica indicar nível crítico, a bomba principal é desligada.
- Se o nível estiver seguro, a bomba principal permanece ligada.
- Se a boia de aviso indicar nível abaixo do ideal, o sistema exibe um alerta, mas mantém a bomba ligada caso a boia crítica ainda esteja em nível seguro.

## Ventoinhas

As ventoinhas são mantidas continuamente ligadas.

Pino utilizado:

```cpp
#define RELE_VENTOINHA 17
```

No `setup()`, o sistema liga as ventoinhas automaticamente.

Durante o `loop()`, o código verifica se a ventoinha permanece ligada. Caso esteja desligada, ela é religada.

## Dosadora peristáltica

A bomba dosadora peristáltica está conectada ao relé no pino:

```cpp
#define RELE_DOSADORA 18
```

No estágio atual, apenas uma dosadora está sendo utilizada.

Ela funciona por temporizador, com o objetivo de dosar água em uma mini estufa de germinação.

Configuração atual:

```cpp
#define TEMPO_DOSADORA_LIGADA_MS 7000UL
#define TEMPO_DOSADORA_DESLIGADA_MS (8UL * 60UL * 60UL * 1000UL)
#define TEMPO_CICLO_DOSADORA_MS (TEMPO_DOSADORA_LIGADA_MS + TEMPO_DOSADORA_DESLIGADA_MS)
```

A lógica atual é:

- A dosadora liga por 7 segundos.
- Depois permanece desligada por 8 horas.
- O ciclo se repete automaticamente.

O controle é feito usando `millis()`, evitando depender de `delay()` para a contagem principal.

## Sensores de água

O sistema trabalha com dois sensores principais relacionados à água:

| Sensor  | Função                           |
| ------- | -------------------------------- |
| DS18B20 | Mede a temperatura da água       |
| TDS/EC  | Mede a condutividade/TDS da água |

A leitura do DS18B20 também é utilizada para compensação da leitura do TDS.

Caso o DS18B20 falhe, o código utiliza `25 °C` como temperatura padrão para o cálculo aproximado do TDS.

## Cálculo aproximado do TDS

O sensor TDS fornece uma leitura analógica pelo pino `GPIO34`.

O ESP32 lê esse valor por ADC e converte para tensão:

```cpp
float tensaoTDS = adcTDS * (VREF / ADC_RESOLUTION);
```

Depois, o valor é compensado pela temperatura da água:

```cpp
float coeficienteCompensacao = 1.0 + 0.02 * (temperaturaAgua - 25.0);
float tensaoCompensada = tensao / coeficienteCompensacao;
```

Por fim, é calculado um valor aproximado de TDS em ppm:

```cpp
float tds =
    (133.42 * tensaoCompensada * tensaoCompensada * tensaoCompensada -
     255.86 * tensaoCompensada * tensaoCompensada +
     857.39 * tensaoCompensada) *
    0.5;
```

O valor ainda precisa de calibração para ser usado com maior precisão.

## Status atual do projeto

Até o momento, foram realizados testes com:

- DHT11
- RTC DS3231
- DS18B20
- TDS/EC
- Sensor de boia
- Módulo relé
- Bomba principal
- Ventoinhas
- Dosadora peristáltica
- Temporizador com `millis()`
- Integração de sensores e atuadores no código principal

## Estado atual da montagem

No estágio atual, o sistema possui:

- Monitoramento ambiental com DHT11
- Relógio em tempo real com RTC DS3231
- Medição da temperatura da água com DS18B20
- Medição aproximada de TDS/condutividade
- Controle de nível por boias
- Bomba principal controlada por nível de água
- Ventoinhas ligadas continuamente
- Uma bomba dosadora peristáltica controlada por temporizador

## Próximos passos

- Testar o sensor de pH
- Calibrar o sensor de pH
- Melhorar a calibração do sensor TDS/EC
- Validar leituras em solução nutritiva real
- Testar o funcionamento prolongado da dosadora
- Organizar o código principal em arquivos separados
- Documentar o esquema final de ligações
- Avaliar uso futuro das outras duas dosadoras peristálticas
- Melhorar a segurança elétrica e a organização dos cabos

## Observações

Este projeto está em desenvolvimento e passa por testes individuais e integrações graduais.

A pasta `testes_individuais` mantém os códigos usados em cada etapa de teste, permitindo consultar versões anteriores e entender a evolução do sistema.
