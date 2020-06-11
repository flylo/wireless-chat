#include "TxRx.h"

TxRx::TxRx(int speed, int rxPin, int txPin)
{
  this->radio = RH_ASK(speed, rxPin, txPin);
  this->rxMsg = "";
}

// TODO: transmit with String as well
bool TxRx::transmit(char *txMsg)
{
  radio.setModeTx();
  while (radio.mode() == RHGenericDriver::RHModeRx)
  {
    Serial.println(F("Waiting..."));
    Serial.println(radio.mode());
    radio.setModeTx();
    delay(500);
  }
  radio.send((uint8_t *)txMsg, strlen(txMsg));
  bool sent = radio.waitPacketSent();
  Serial.println(F("Message Sent: "));
  Serial.println(sent);
  return sent;
}

String TxRx::getReceiveMsg()
{
  Serial.println(F("getter"));
  Serial.println(rxMsg);
  return rxMsg;
}

bool TxRx::tryReceive()
{
  radio.setModeRx();
  while (radio.mode() == RHGenericDriver::RHModeTx)
  {
    Serial.println(F("Waiting..."));
    Serial.println(radio.mode());
    radio.setModeRx();
    delay(500);
  }
  uint8_t receive_buffer[32];
  uint8_t buflen = sizeof(receive_buffer);
  if (radio.recv(receive_buffer, &buflen))
  {
    rxMsg = String((char *)receive_buffer);
    Serial.println(F("Message Received"));
    Serial.println(rxMsg);
    return true;
  }
  return false;
}

char TxRx::getMode()
{
  return radio.mode();
}

void TxRx::init()
{
 if (!radio.init())
 {
   Serial.println(F("Radio module failed to initialize"));
 } else
 {
   Serial.println(F("Radio module initialized successfully"));
 }
 // Try to receive to clear out the buffer
 tryReceive();
}
