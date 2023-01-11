#include <Arduino.h>
// Wire Master Writer by Nicholas Zambetti http://www.zambetti.com
#include <Wire.h>

int8_t output = 0;

void setup()
{
  Wire.begin(2);      // inicia i2c bus - mestre é opcional ter endereço
  Serial.begin(9600); // inicia canal serial para saída

  pinMode(LED_BUILTIN, OUTPUT);
}

void requestEvent()
{
  Wire.write(output);

  Serial.println(output);
}
