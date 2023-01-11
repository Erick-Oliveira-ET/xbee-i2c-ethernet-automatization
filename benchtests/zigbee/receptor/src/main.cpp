#include <Arduino.h>
#include <XBee.h>
// 0013A200 40E8B8A3

XBee xbee = XBee();
ZBRxResponse rx = ZBRxResponse();

int LED_PORT = 8;
int ledPortState = 0;
int ledBuiltinState = 0;

void setup()
{
  Serial.begin(9600);
  xbee.setSerial(Serial);

  pinMode(LED_PORT, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  char sample;
  xbee.readPacket();
  if (xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {
      xbee.getResponse().getZBRxResponse(rx);

      sample = (char)rx.getData(0);

      Serial.print("Sou receptor: ");
      Serial.println(sample);

      if (sample == '7')
      {
        Serial.println("Liga");
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        Serial.println("Desliga");
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }
}