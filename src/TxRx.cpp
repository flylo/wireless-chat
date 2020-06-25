#include "TxRx.h"

char rxMsg[RH_NRF24_MAX_MESSAGE_LEN];
RH_NRF24 radio;
AES128 aes = AES128();
RHEncryptedDriver encryptedDriver(radio, aes);

TxRx::TxRx(char *PIN)
{
  aes.setKey(PIN, strlen(PIN));
}

bool TxRx::transmit(char *txMsg)
{
  // TODO: acking logic
  // radio.send((uint8_t *)"LOL");
  // radio.waitAvailableTimeout(6000);
  // uint8_t ackPinBuffer[16];
  // uint8_t len = sizeof(ackPinBuffer);
  // if (radio.recv(ackPinBuffer, &len))
  // {
  //   int i = 0;
  //   while (i < len)
  //   {
  //     // TODO: finish...
  //     char c = ackPinBuffer[i];
  //     if ()
  //   }
  // }
  encryptedDriver.send((uint8_t *)txMsg, strlen(txMsg));
  bool sent = encryptedDriver.waitPacketSent();
  return sent;
}

char *TxRx::getReceiveMsg()
{
  return rxMsg;
}

bool TxRx::tryReceive()
{
  uint8_t receive_buffer[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(receive_buffer);
  if (encryptedDriver.recv(receive_buffer, &buflen))
  {
    // TODO: make sure we use this everywhere
    for (int i = 0; i < RH_NRF24_MAX_MESSAGE_LEN; i++)
    {
      char c = (char)receive_buffer[i];
      // https://www.december.com/html/spec/ascii.html
      // We remove NUL, SOH, STX, ETX
      if (int(c) > 3)
      {
        rxMsg[i] = c;
      }
    }
    return true;
  }
  return false;
}

void TxRx::init()
{
  if (!radio.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!radio.setChannel(1))
    Serial.println("setChannel failed");
  if (!radio.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
  // Try to receive to clear out the buffer
  tryReceive();
}

void TxRx::clear()
{
  for (int i = 0; i < 32; i++)
  {
    rxMsg[i] = '\0';
  }
}
