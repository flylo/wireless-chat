#include "TxRx.h"
const char SYN_CHAR = '\26';
const char *ACK = "\6\0";
const char *NAK = "\25\0";
char rxMsg[RH_NRF24_MAX_MESSAGE_LEN];
RH_NRF24 radio;
AES128 aes = AES128();
RHEncryptedDriver encryptedDriver(radio, aes);
RHReliableDatagram manager(encryptedDriver, CLIENT_ADDRESS);
TxRx::TxRx()
{
}

bool _transmit(char *txMsg)
{
  // add the SYN byte
  char msgWithSyn[31];
  msgWithSyn[0] = SYN_CHAR;
  for (int i = 1; i < 31; i++)
  {
    msgWithSyn[i] = txMsg[i - 1];
  }
  encryptedDriver.send((uint8_t *)msgWithSyn, strlen(msgWithSyn));
  bool sent = encryptedDriver.waitPacketSent();
  if (sent)
  {
    encryptedDriver.setMode(RHGenericDriver::RHModeRx);
    delay(3000);
    Serial.println("mode");
    Serial.println(encryptedDriver.mode());

    // if (!encryptedDriver.waitAvailableTimeout(3000))
    // {
    //   Serial.println("timeout waiting for receive ack");
    //   return false;
    // };
    uint8_t ackResponse[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(ackResponse);
    if (encryptedDriver.recv(ackResponse, &len))
    {
      char respByte = (char)ackResponse[0];
      Serial.println("rbyte");
      Serial.println(respByte);
      if (respByte == ACK[0])
      {
        Serial.println("ACKED");
        return true;
      }
      else
      {
        Serial.println("NOT ACKED");
        return false;
      }
    }
  }
  return false;
}

bool TxRx::transmit(char *txMsg)
{
  int retries = 0;
  while (retries < 2)
  {
    if (_transmit(txMsg))
    {
      return true;
    }
    else
    {
      retries++;
    }
  }
  return false;
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
    Serial.println("LOL");
    char syn = (char)receive_buffer[0];
    if (syn != SYN_CHAR)
    {
      delay(500); // pause to allow for client to switch back to RX mode
      for (int iters = 0; iters < 3; iters++)
      {
        encryptedDriver.send((uint8_t *)NAK, strlen(NAK));
        encryptedDriver.waitPacketSent();
        delay(100);
      }
    }
    // TODO: make sure we use this everywhere
    for (int i = 1; i < RH_NRF24_MAX_MESSAGE_LEN; i++)
    {
      char c = (char)receive_buffer[i];
      // https://www.december.com/html/spec/ascii.html
      // We remove NUL, SOH, STX, ETX
      if (int(c) > 3)
      {
        rxMsg[i - 1] = c;
      }
    }
    // ACK the message
    // NOTE: seems like ACKing a few times works better... TX->RX switching is slow i guess
    Serial.println("sending ACK");
    delay(500); // pause to allow for client to switch back to RX mode
    for (int iters = 0; iters < 3; iters++)
    {
      encryptedDriver.send((uint8_t *)ACK, strlen(ACK));
      encryptedDriver.waitPacketSent();
      delay(100);
    }
    return true;
  }
  return false;
}

void TxRx::clear()
{
  for (int i = 0; i < 32; i++)
  {
    rxMsg[i] = '\0';
  }
}

void TxRx::init(char *PIN)
{
  aes.setKey(PIN, 16);
  if (!encryptedDriver.init())
    Serial.println("init failed");
  // Channel 2 and 
  if (!radio.setChannel(2))
    Serial.println("setChannel failed");
  if (!radio.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPowerm12dBm))
    Serial.println("setRF failed");
  // Try to receive to clear out the buffer
  tryReceive();
}
