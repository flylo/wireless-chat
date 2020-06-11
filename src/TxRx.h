#ifndef TX_RX_H
#define TX_RX_H
#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

class TxRx
{

private:
  RH_ASK radio;
  String rxMsg;

public:
  TxRx(int speed, int rxPin, int txPin);
  bool tryReceive();
  String getReceiveMsg();
  bool transmit(char *txMsg);
  void init();
  char getMode();
};

#endif
