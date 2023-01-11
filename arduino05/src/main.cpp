#include <Arduino.h>
#include <Wire.h>
#include <XBee.h>
#define LED_PORT 8
#define VALVE_PORT 9              // port to activate a spring returned solenoid control valve
#define ARDUINO_B_SENSOR_I2C_ID 4 // address of arduino4 in I2C communication with arduino 5
#define XBEE_REQUEST_ADDRESS_SH 0x0013A200
#define XBEE_REQUEST_ADDRESS_SL 0x40C1B1F0

XBee xbee = XBee();
ZBRxResponse rx = ZBRxResponse();

// Specify the address of the remote XBee (this is the SH + SL)
XBeeAddress64 add64 = XBeeAddress64(XBEE_REQUEST_ADDRESS_SH,
                                    XBEE_REQUEST_ADDRESS_SL);

int bState = 0;
uint8_t step = 0;
uint8_t stepA = 0;
int ledPortState = 0;
int ledBuiltinState = 0;
// int SEQ_B[5] = {0, 0, 1, -1, 0}; // SEQ = { A+, A-, B+, A+B- , A- }
int SEQ_B[6] = {0, 0, 0, -1, 1, 0}; // SEQ = { A+, A-, B+, A+B- , A- }
int SEQ_B_SIZE = 6;

/**
 * Turns the rawState received by the Arduino in code
 * and turn it into piston commands (1, 0, -1)
 */
int stateToPositionMap(int rawState)
{
  if (rawState == 0)
    return 0;
  else if (rawState == 1)
    return 1;
  else if (rawState == 2)
    return -1;
}

/**
 * Because the system is automatic, the steps return to zero
 * after the steps hit max value
 */
void nextStep()
{
  delay(1000);
  if (step != SEQ_B_SIZE - 1)
  {
    step++;
  }
  else
  {

    step = 0;
  }
}

/**
 * Gets state of the B piston from Arduino 4
 */
int8_t getBState()
{
  int8_t temp;

  Wire.requestFrom(ARDUINO_B_SENSOR_I2C_ID, 8); // request 8 bytes from peripheral device ARDUINO_B_SENSOR_I2C_ID

  // peripheral may send less than requested
  temp = Wire.read(); // receive a byte as character

  return temp;
}

void sendStepToA()
{
  ZBTxRequest data = ZBTxRequest(add64, &step, sizeof(step));
  xbee.send(data);
}

int8_t getStepFromA()
{
  xbee.readPacket();
  if (xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {
      xbee.getResponse().getZBRxResponse(rx);
      stepA = rx.getData(0);
      Serial.println("stepA: " + stepA);
    }
  }

  return stepA;
}

void pistonForward()
{
  digitalWrite(LED_BUILTIN, 1);
  digitalWrite(VALVE_PORT, 0);
}

void pistonReturn()
{
  digitalWrite(LED_BUILTIN, 0);
  digitalWrite(VALVE_PORT, 1);
}

/**
 * Recieves a piston command (1, 0, -1) and
 * execute the correct move in the piston
 */
void movePiston(int command)
{
  // do nothing
  if (command == 0)
    return;

  // go forward
  if (command == 1)
  {
    pistonForward();
  }
  // return
  else if (command == -1)
  {
    pistonReturn();
  }
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  xbee.setSerial(Serial);

  pinMode(LED_PORT, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VALVE_PORT, OUTPUT);
}

void loop()
{
  if (getStepFromA() == -1)
  {
    return;
  }

  int8_t bStateCode;

  bStateCode = getBState();

  bState = stateToPositionMap(bStateCode);

  Serial.print("bStateCode: ");
  Serial.print(bStateCode);
  Serial.print(", step: ");
  Serial.print(step);
  Serial.print(", SEQ_B[step]: ");
  Serial.print(SEQ_B[step]);
  Serial.print(", bState: ");
  Serial.println(bState);

  // if the state of the piston is not equal to the required state
  // send signal to go to that state
  if (SEQ_B[step] != 0 && SEQ_B[step] != bState)
  {
    movePiston(SEQ_B[step]);
    return; // the equivalent of continue for the arduino loop function
  }

  nextStep();

  while (step != stepA)
  {
    sendStepToA();
    getStepFromA();
  }
}