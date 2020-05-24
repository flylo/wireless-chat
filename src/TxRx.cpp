#include "TxRx.h"

TxRx::TxRx(int speed, int rxPin, int txPin)
{
  this->radio = RH_ASK(speed, rxPin, txPin);
  this->rxMsg = new char[32];
  init();
}

bool TxRx::transmit(char *txMsg)
{
  radio.send((uint8_t *)txMsg, strlen(txMsg));
  return radio.waitPacketSent();
}

char *TxRx::getReceiveMsg()
{
  return rxMsg;
}

bool TxRx::tryReceive()
{
  uint8_t receive_buffer[32];
  uint8_t buflen = sizeof(receive_buffer);
  if (radio.recv(receive_buffer, &buflen))
  {
    char *received = (char *)receive_buffer;
    rxMsg = received;
    return true;
  }
  return false;
}

void TxRx::init()
{
 if (!radio.init())
 {
   Serial.println("Radio module failed to initialize");
 } else
 {
   Serial.println("Radio module initialized successfully");
 }
}