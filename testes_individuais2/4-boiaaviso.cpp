#include <Arduino.h>

// -------------------- PINOS --------------------
#define BOIA_AVISO 27 // D27

// -------------------- LÓGICA --------------------
// 0 = nível abaixando
// 1 = nível OK
#define AVISO_NIVEL_BAIXO LOW
#define AVISO_NIVEL_OK HIGH

// -------------------- VARIÁVEIS --------------------
int estadoAnterior = -1;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(BOIA_AVISO, INPUT_PULLUP);

  Serial.println("Teste boia aviso iniciado");
}

void loop()
{
  int aviso = digitalRead(BOIA_AVISO);

  if (aviso != estadoAnterior)
  {
    estadoAnterior = aviso;

    Serial.print("Aviso: ");
    Serial.print(aviso);
    Serial.print(" | ");

    if (aviso == AVISO_NIVEL_BAIXO)
    {
      Serial.println("NIVEL ABAIXANDO");
    }
    else
    {
      Serial.println("NIVEL OK");
    }
  }

  delay(100);
}
