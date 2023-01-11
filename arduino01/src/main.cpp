#include "Arduino.h"
#include "SPI.h"
#include "Ethernet.h"
#include "EthernetUdp.h"
#define BUTTON_PIN 2

EthernetUDP Udp;
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x64, 0x44};
byte ip[] = {192, 168, 1, 131};
byte remoteip[] = {192, 168, 1, 151};

unsigned int port = 8888;

int8_t isActive = 0;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
int led = LED_BUILTIN;

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Ethernet.begin(mac, ip);
  Udp.begin(port);
  Serial.begin(9600);
}

void loop()
{

  int buttonState = digitalRead(BUTTON_PIN);

  // TODO: verify if the button is pullup or pulldown
  if (!buttonState)
  {
    // Avoid uncontrolled changes If the button keeps being pressed
    while (!digitalRead(BUTTON_PIN))
    {
    }

    isActive = !isActive;
  }

  Serial.print("isActive: ");
  Serial.println(isActive);

  Udp.beginPacket(remoteip, port);
  Udp.write(isActive);
  Udp.endPacket();
}