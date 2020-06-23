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
  bool transmit(char *txMsg);
  char *getReceiveMsg();
  void init();
  void clear();
};

#endif
