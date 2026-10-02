#include <Arduino.h>

// -------------------- PINOS --------------------
#define BOIA_AVISO 27
#define BOIA_CRITICA 14
#define LED_BOMBA 2

// -------------------- LÓGICA DAS BOIAS --------------------
// Boia aviso:
// 0 = nível abaixando
// 1 = nível OK
#define AVISO_NIVEL_BAIXO LOW
#define AVISO_NIVEL_OK HIGH

// Boia crítica:
// Pelo seu teste, caída está lendo 1.
// Então:
// 1 = nível crítico / bomba desliga
// 0 = nível ainda seguro / bomba funciona
#define CRITICA_NIVEL_CRITICO HIGH
#define CRITICA_NIVEL_OK LOW

// -------------------- VARIÁVEIS --------------------
int ultimoAviso = -1;
int ultimaCritica = -1;
int ultimoEstadoBomba = -1;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(BOIA_AVISO, INPUT_PULLUP);
  pinMode(BOIA_CRITICA, INPUT_PULLUP);
  pinMode(LED_BOMBA, OUTPUT);

  digitalWrite(LED_BOMBA, LOW);

  Serial.println("Teste conjunto - boia aviso + boia critica");
}

void loop()
{
  int aviso = digitalRead(BOIA_AVISO);
  int critica = digitalRead(BOIA_CRITICA);

  int bombaLigada;

  // -------------------- LÓGICA PRINCIPAL --------------------
  if (critica == CRITICA_NIVEL_CRITICO)
  {
    bombaLigada = 0;
    digitalWrite(LED_BOMBA, LOW);
  }
  else
  {
    bombaLigada = 1;
    digitalWrite(LED_BOMBA, HIGH);
  }

  // -------------------- PRINTA SOMENTE QUANDO MUDA --------------------
  if (aviso != ultimoAviso || critica != ultimaCritica || bombaLigada != ultimoEstadoBomba)
  {
    ultimoAviso = aviso;
    ultimaCritica = critica;
    ultimoEstadoBomba = bombaLigada;

    Serial.print("Aviso: ");
    Serial.print(aviso);

    Serial.print(" | Critica: ");
    Serial.print(critica);

    Serial.print(" | ");

    if (bombaLigada == 0)
    {
      Serial.println("BOMBA DESLIGADA - NIVEL CRITICO");
    }
    else
    {
      if (aviso == AVISO_NIVEL_BAIXO)
      {
        Serial.println("BOMBA FUNCIONANDO - AVISO: NIVEL ABAIXANDO");
      }
      else
      {
        Serial.println("BOMBA FUNCIONANDO - NIVEL OK");
      }
    }
  }

  delay(100);
}