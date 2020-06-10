#include "TxRx.h"

TxRx::TxRx(int speed, int rxPin, int txPin)
{
  this->radio = RH_ASK(speed, rxPin, txPin);
  this->rxMsg = new char[32];
}

// TODO: transmit with String as well
bool TxRx::transmit(char *txMsg)
{
  radio.setModeTx();
  while (radio.mode() == RHGenericDriver::RHModeRx)
  {
    Serial.println("Waiting...");
    Serial.println(radio.mode());
    radio.setModeTx();
    delay(500);
  }
  radio.send((uint8_t *)txMsg, strlen(txMsg));
  bool sent = radio.waitPacketSent();
  // Serial.println("Message Sent: ");
  // Serial.println(sent);
  return sent;
}

char *TxRx::getReceiveMsg()
{
  return rxMsg;
}

bool TxRx::tryReceive()
{
  radio.setModeRx();
  // Serial.println("Available");
  // Serial.println(radio.available());

  // Serial.println("rx good");
  // Serial.println(radio.rxGood());
  // Serial.println("rx bad");
  // Serial.println(radio.rxBad());

  while (radio.mode() == RHGenericDriver::RHModeTx)
  {
    Serial.println("Waiting...");
    Serial.println(radio.mode());
    radio.setModeRx();
    delay(500);
  }
  uint8_t receive_buffer[32];
  uint8_t buflen = sizeof(receive_buffer);
  if (radio.recv(receive_buffer, &buflen))
  {
    char *received = (char *)receive_buffer;
    rxMsg = received;
    Serial.println("Message Received");
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
   Serial.println("Radio module failed to initialize");
 } else
 {
   Serial.println("Radio module initialized successfully");
 }
}
