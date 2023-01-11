#include <Arduino.h>
#include <XBee.h>
#define BUTTON_INPUT 10

XBee xbee = XBee();

XBeeAddress64 add64 = XBeeAddress64(0x0013A200,
                                    0x40C1B208);
uint8_t payload[] = {'1'};

int ledBuiltinState = 0;

void setup()
{
  Serial.begin(9600);
  xbee.setSerial(Serial);

  pinMode(BUTTON_INPUT, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if (digitalRead(BUTTON_INPUT))
  {
    payload[0] = {'7'};
    digitalWrite(LED_BUILTIN, 1);
  }
  else
  {
    payload[0] = {'0'};
    digitalWrite(LED_BUILTIN, 0);
  }

  ZBTxRequest data = ZBTxRequest(add64, payload, sizeof(payload));
  xbee.send(data);
  delay(1000);
}
