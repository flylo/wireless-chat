#include <Arduino.h>
#include <TxRx.h>
#include <DisplayInterface.h>

TxRx txRx = TxRx(2000, 11, 12);
DisplayInterface displayInterface = DisplayInterface();

void setup()
{
  Serial.begin(9600);
  txRx.init();
  displayInterface.init();
}

void loop()
{
  Serial.println("Try to receive");
  if (txRx.tryReceive())
  {
    displayInterface.displayFullMessage(txRx.getReceiveMsg());
  }
  delay(3000);
  Serial.println("Try to transmit");
  txRx.transmit("Hello World");
  delay(3000);
  // else
  // {
  //   txRx.transmit("Hello World");
  // }
}
