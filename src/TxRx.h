#ifndef TX_RX_H
#define TX_RX_H
#include <Arduino.h>
#include <RH_NRF24.h>
#include <SPI.h>

class TxRx
{

private:
  RH_NRF24 radio;

public:
  TxRx();
  bool tryReceive();
  char *getReceiveMsg();
  bool transmit(char *txMsg);
  void init();
  char getMode();
};

#endif
