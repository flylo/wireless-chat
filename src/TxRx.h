#ifndef TX_RX_H
#define TX_RX_H
#include <Arduino.h>
#include <RH_NRF24.h>
#include <RHEncryptedDriver.h>
#include <SPI.h>
#include <AES.h>
#include <RHReliableDatagram.h>

#define DEFAULT_ADDR 0

class TxRx
{

public:
  TxRx();
  bool tryReceive();
  bool transmit(char *txMsg);
  char *getReceiveMsg();
  void init(char *PIN);
  void clear();
};

#endif
