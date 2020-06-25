#ifndef TX_RX_H
#define TX_RX_H
#include <Arduino.h>
#include <RH_NRF24.h>
#include <RHEncryptedDriver.h>
#include <SPI.h>
// TODO: need?
#include <AES.h>

class TxRx
{

// private:
  // RH_NRF24 radio;

public:
  TxRx(char* PIN);
  bool tryReceive();
  bool transmit(char *txMsg);
  char *getReceiveMsg();
  void init();
  void clear();
};

#endif
