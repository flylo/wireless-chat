#include <Arduino.h>
#include <TxRx.h>
#include <MessageDisplay.h>

TxRx txRx = TxRx(2000, 11, 12);
DisplayInterface displayInterface = DisplayInterface();

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (txRx.tryReceive())
  {
    displayInterface.displayFullMessage(txRx.getReceiveMsg());
  } else
  {
    txRx.transmit("Hello World");
  }
  Serial.println("WOOO");
}
