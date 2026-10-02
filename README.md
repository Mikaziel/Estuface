# Estuface - ESP32

Projeto de automação para cultivo hidropônico NFT utilizando ESP32, sensores ambientais, sensores de água, boias de nível e atuadores controlados por relé.

O objetivo do sistema é monitorar variáveis importantes do cultivo, como temperatura e umidade do ambiente, temperatura da água e condutividade/TDS da solução, além de controlar bomba principal, ventoinhas e uma bomba dosadora peristáltica por temporizador.

---

## Plataforma utilizada

- VS Code
- PlatformIO
- ESP32 DevKit V1 / ESP32 compatível com `esp32dev`
- Framework Arduino
- Monitor Serial em 115200 baud

---

## Observação sobre a nomenclatura dos pinos

Nas versões anteriores da documentação, os pinos eram descritos usando a nomenclatura `GPIO`, como `GPIO4`, `GPIO23` e `GPIO34`.

A partir desta etapa, a documentação passa a usar principalmente os nomes impressos na placa ESP32, como `D4`, `D23` e `D34`.

No código, os pinos continuam sendo declarados pelo número correspondente ao nome da placa:

```cpp
#define DHT_PIN 4       // D4
#define DS18B20_PIN 23  // D23
#define TDS_PIN 34      // D34
```

---

## Estrutura atual do projeto

```txt
Estuface
├── include
│   └── README
├── lib
│   └── README
├── platformio.ini
├── README.md
├── src
│   └── main.cpp
├── test
│   └── README
├── testes_individuais
│   ├── 1-dht11.cpp
│   ├── 10-x88.cpp
│   ├── 100-x88xy7.cpp
│   ├── 101-temporizador.cpp
│   ├── 11-xy7.cpp
│   ├── 12-x-bombarele.cpp
│   ├── 13-xy-bombaventoinha.cpp
│   ├── 2-rtc.cpp
│   ├── 3-tempagua.cpp
│   ├── 4-1&2.cpp
│   ├── 5-4&3.cpp
│   ├── 6-sensortds.cpp
│   ├── 7-5&6.cpp
│   ├── 8-sensorboia.cpp
│   ├── 9-x8.cpp
│   ├── dosadora.cpp
│   ├── dosadoramain.cpp
│   ├── emojis.cpp
│   └── esp32-ph.cpp
└── testes2
    ├── 1-dosadora.cpp
    ├── 2-dht11ertc.cpp
    ├── 3-tempagua.cpp
    ├── 4-boiaaviso.cpp
    ├── 5-boiacritica.cpp
    ├── 6-4e5.cpp
    └── teste1.cpp
```

A pasta `src` contém o código principal que será compilado e enviado para o ESP32.

A pasta `testes_individuais` mantém códigos utilizados em etapas anteriores do projeto, servindo como histórico dos testes feitos durante o desenvolvimento.

A pasta `testes2` contém os testes mais recentes e corrigidos, usando a nomenclatura atual dos pinos da placa, como `D4`, `D23`, `D27`, `D14` e `D18`.

---

## Testes atuais

Os testes mais recentes estão na pasta `testes2`.

| Arquivo | Função |
| ------- | ------ |
| `1-dosadora.cpp` | Testa a bomba dosadora peristáltica no D18 |
| `2-dht11ertc.cpp` | Testa DHT11 no D4 e RTC DS3231 em D25/D26 |
| `3-tempagua.cpp` | Testa o DS18B20 no D23 |
| `4-boiaaviso.cpp` | Testa a boia de aviso no D27 |
| `5-boiacritica.cpp` | Testa a boia crítica no D14 |
| `6-4e5.cpp` | Testa as duas boias simulando a bomba principal |
| `teste1.cpp` | Teste auxiliar da nova placa ESP32 |

---

## Sensores e módulos trabalhados

| Sensor/Módulo  | Função                                 | Status   |
| -------------- | -------------------------------------- | -------- |
| DHT11          | Mede temperatura e umidade do ambiente | Testado  |
| RTC DS3231     | Mantém data e hora em tempo real       | Testado  |
| DS18B20        | Mede a temperatura da água             | Testado  |
| TDS/EC         | Mede condutividade/TDS da água         | Testado  |
| Boia de aviso  | Indica nível abaixo do ideal           | Testado  |
| Boia crítica   | Indica nível crítico                   | Testado  |
| Sensor de pH   | Mede o pH da água/solução nutritiva    | Pendente |

---

## Pinos dos sensores

| Sensor/Módulo | Pino do sensor | Pino na placa |
| ------------- | -------------- | ------------- |
| DHT11         | DATA           | D4            |
| RTC DS3231    | SDA            | D25           |
| RTC DS3231    | SCL            | D26           |
| DS18B20       | DATA           | D23           |
| TDS/EC        | A / Analógico  | D34           |
| Boia de aviso | Sinal          | D27           |
| Boia crítica  | Sinal          | D14           |
| Sensor de pH  | A / Analógico  | D35           |

---

## Pinos dos atuadores

| Atuador               | Pino na placa | Canal do relé |
| --------------------- | ------------- | ------------- |
| Bomba principal       | D32           | IN1           |
| Ventoinhas            | D33           | IN2           |
| Dosadora peristáltica | D18           | IN3           |

Inicialmente estavam previstos pinos para três bombas dosadoras peristálticas, porém no estágio atual do projeto apenas uma dosadora está sendo utilizada.

A dosadora atual funciona como temporizador para dosagem em uma mini estufa de germinação.

---

## Ligações principais

### DHT11

| Pino do DHT11 | Ligação |
| ------------- | ------- |
| VCC           | 3V3     |
| GND           | GND     |
| DATA          | D4      |

---

### RTC DS3231

| Pino do RTC | Ligação |
| ----------- | ------- |
| VCC         | 3V3     |
| GND         | GND     |
| SDA         | D25     |
| SCL         | D26     |

O RTC foi movido dos pinos padrão D21/D22 para D25/D26, liberando D21 para uso futuro.

---

### DS18B20

| Fio do DS18B20 | Ligação |
| -------------- | ------- |
| Vermelho       | 3V3     |
| Preto          | GND     |
| Amarelo        | D23     |

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
3V3 / fio vermelho ---- resistor ---- fio amarelo / DATA / D23
```

Durante os testes, foi identificado um problema de mau contato/fiação nas pontas do sensor. Após cortar as pontas dos fios e refazer a soldagem, o sensor passou a funcionar corretamente.

---

### TDS/EC

| Pino do módulo TDS | Ligação |
| ------------------ | ------- |
| +                  | 3V3     |
| -                  | GND     |
| A                  | D34     |

O sensor TDS/EC é analógico e utiliza o pino D34 para leitura.

Apenas a sonda deve entrar em contato com a água. A placa do módulo TDS não deve ser molhada.

---

### Boias

| Boia          | Pino na placa | Função                       |
| ------------- | ------------- | ---------------------------- |
| Boia de aviso | D27           | Indica nível abaixo do ideal |
| Boia crítica  | D14           | Indica nível crítico         |

As boias são usadas com `INPUT_PULLUP`.

Ligação básica de cada boia:

```txt
Um fio da boia -> GND
Outro fio      -> pino de sinal
```

### Lógica da boia de aviso

```txt
1 = nível OK
0 = nível abaixando
```

Quando a boia de aviso indica nível abaixando, o sistema exibe um alerta, mas a bomba principal continua funcionando se a boia crítica ainda indicar nível seguro.

### Lógica da boia crítica

```txt
0 = nível seguro
1 = nível crítico
```

Quando o nível crítico é detectado, a bomba principal é desligada para evitar funcionamento seco.

---

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

---

## Função das bibliotecas

| Biblioteca                | Função                                            |
| ------------------------- | ------------------------------------------------- |
| `RTClib`                  | Comunicação e leitura do RTC DS3231               |
| `Adafruit BusIO`          | Dependência utilizada por bibliotecas da Adafruit |
| `DHT sensor library`      | Leitura do sensor DHT11                           |
| `Adafruit Unified Sensor` | Dependência utilizada pela biblioteca do DHT      |
| `OneWire`                 | Comunicação OneWire utilizada pelo DS18B20        |
| `DallasTemperature`       | Leitura de temperatura do DS18B20                 |

---

## Configuração do `platformio.ini`

Configuração utilizada no projeto:

```ini
[platformio]
default_envs = esp32

[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 115200

lib_deps =
    adafruit/RTClib
    adafruit/Adafruit BusIO
    adafruit/DHT sensor library
    adafruit/Adafruit Unified Sensor
    paulstoffregen/OneWire
    milesburton/DallasTemperature
```

---

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

---

## Bomba principal

A bomba principal será controlada pelas boias de nível.

Pino previsto:

```cpp
#define RELE_BOMBA 32 // D32
```

Lógica:

- Se a boia crítica indicar nível crítico, a bomba principal é desligada.
- Se o nível estiver seguro, a bomba principal permanece ligada.
- Se a boia de aviso indicar nível abaixo do ideal, o sistema exibe um alerta, mas mantém a bomba ligada caso a boia crítica ainda esteja em nível seguro.

Durante os testes atuais, a bomba principal foi simulada pelo LED interno do ESP32.

Lógica validada:

```txt
Aviso: 1 | Critica: 0 -> bomba funcionando / nível OK
Aviso: 0 | Critica: 0 -> bomba funcionando / aviso de nível abaixando
Aviso: 1 | Critica: 1 -> bomba desligada / nível crítico
Aviso: 0 | Critica: 1 -> bomba desligada / nível crítico
```

---

## Ventoinhas

As ventoinhas serão mantidas continuamente ligadas.

Pino previsto:

```cpp
#define RELE_VENTOINHA 33 // D33
```

No funcionamento integrado, as ventoinhas devem ser ligadas automaticamente no `setup()` e permanecer ligadas durante a execução do sistema.

---

## Dosadora peristáltica

A bomba dosadora peristáltica está conectada ao relé no pino:

```cpp
#define RELE_DOSADORA 18 // D18
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

No código de teste, o LED interno do ESP32 também é usado para indicar quando a dosadora está ligada.

---

## Sensores de água

O sistema trabalha com dois sensores principais relacionados à água:

| Sensor  | Função                           |
| ------- | -------------------------------- |
| DS18B20 | Mede a temperatura da água       |
| TDS/EC  | Mede a condutividade/TDS da água |

A leitura do DS18B20 também será utilizada para compensação da leitura do TDS.

Caso o DS18B20 falhe, o código pode utilizar `25 °C` como temperatura padrão para o cálculo aproximado do TDS.

---

## Cálculo aproximado do TDS

O sensor TDS fornece uma leitura analógica pelo pino D34.

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

---

## Status atual do projeto

Até o momento, foram realizados testes com:

- DHT11
- RTC DS3231
- DS18B20
- TDS/EC
- Boia de aviso
- Boia crítica
- Simulação da bomba principal com as duas boias
- Dosadora peristáltica
- Temporizador com `millis()`

---

## Estado atual da montagem

No estágio atual, o sistema possui testes individuais funcionais para:

- Monitoramento ambiental com DHT11
- Relógio em tempo real com RTC DS3231
- Medição da temperatura da água com DS18B20
- Medição aproximada de TDS/condutividade
- Controle de nível por boias
- Simulação da bomba principal por lógica de nível
- Uma bomba dosadora peristáltica controlada por temporizador

---

## Próximos passos

- Testar o relé da bomba principal no D32
- Testar o relé das ventoinhas no D33
- Integrar boias com o relé real da bomba principal
- Integrar sensores de água: DS18B20 + TDS/EC
- Integrar sensores ambientais: DHT11 + RTC DS3231
- Testar o sensor de pH
- Calibrar o sensor de pH
- Melhorar a calibração do sensor TDS/EC
- Validar leituras em solução nutritiva real
- Testar o funcionamento prolongado da dosadora
- Organizar o código principal em arquivos separados
- Documentar o esquema final de ligações
- Avaliar uso futuro das outras duas dosadoras peristálticas
- Melhorar a segurança elétrica e a organização dos cabos

---

## Observações

Este projeto está em desenvolvimento e passa por testes individuais e integrações graduais.

A pasta `testes_individuais` mantém códigos utilizados em etapas anteriores do projeto, permitindo consultar versões anteriores e entender a evolução do sistema.

A pasta `testes2` concentra os testes mais recentes feitos durante a reorganização atual do projeto.

O README atual é uma versão intermediária da documentação. A versão final deve ser revisada após a integração completa dos sensores, boias, relés e atuadores reais.