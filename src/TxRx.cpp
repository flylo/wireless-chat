#include "TxRx.h"

char rxMsg[32];
TxRx::TxRx(int speed, int rxPin, int txPin)
{
}

// TODO: transmit with String as well
bool TxRx::transmit(char *txMsg)
{
  // radio.setModeTx();
  // while (radio.mode() == RHGenericDriver::RHModeRx)
  // {
  //   radio.setModeTx();
  //   delay(500);
  // }
  radio.send((uint8_t *)txMsg, strlen(txMsg));
  bool sent = radio.waitPacketSent();
  return sent;
}

char *TxRx::getReceiveMsg()
{
  return *rxMsg;
}

bool TxRx::tryReceive()
{
  // radio.setModeRx();
  // while (radio.mode() == RHGenericDriver::RHModeTx)
  // {
  //   radio.setModeRx();
  //   delay(500);
  // }
  uint8_t receive_buffer[32];
  uint8_t buflen = sizeof(receive_buffer);
  if (radio.recv(receive_buffer, &buflen))
  {
    for (int i = 0; i < 32; i++)
    {
      rxMsg[i] = receive_buffer[i];
    }
    // rxMsg = (char *)receive_buffer;
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
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!radio.setChannel(1))
    Serial.println("setChannel failed");
  if (!radio.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
  // Try to receive to clear out the buffer
  tryReceive();
}
