#include <Arduino.h>
// Wire Master Writer by Nicholas Zambetti http://www.zambetti.com
#include <Wire.h>

void setup()
{
  Wire.begin();       // inicia i2c bus - mestre é opcional ter endereço
  Serial.begin(9600); // inicia canal serial para saída

  pinMode(LED_BUILTIN, OUTPUT);
}

int ledState = 0;

void loop()
{
  Wire.requestFrom(2, sizeof(int8_t)); // requer 6 bytes do escravo #2

  int payload;

  while (Wire.available()) // escravo deve enviar mensagem até o tamanho definido
  {
    int payload = Wire.read(); // receibe um byte como caracter
  }

  Serial.println(payload);

  if (payload == 1)
    ledState = 1;

  digitalWrite(LED_BUILTIN, ledState);

  delay(500);
}
