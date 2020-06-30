#include "TxRx.h"
const int GLOBAL_TX_RETRIES = 0;
const char SYN_CHAR = '\26';
const char *ACK = "\6\0";
const char *NAK = "\25\0";
char rxMsg[RH_NRF24_MAX_MESSAGE_LEN];
RH_NRF24 radio;
AES128 aes = AES128();
RHEncryptedDriver encryptedDriver(radio, aes);
RHReliableDatagram reliableRadio(encryptedDriver, DEFAULT_ADDR);

TxRx::TxRx()
{
}

TxRx::ConfirmationType _transmit(char *txMsg)
{
  // add the SYN byte
  char msgWithSyn[31];
  msgWithSyn[0] = SYN_CHAR;
  for (int i = 1; i < 31; i++)
  {
    msgWithSyn[i] = txMsg[i - 1];
  }
  Serial.println("sending");
  Serial.println(msgWithSyn);
  bool sent = reliableRadio.sendtoWait((uint8_t *)msgWithSyn, strlen(msgWithSyn), DEFAULT_ADDR);
  if (sent)
  {
    uint8_t ackResponse[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(ackResponse);
    if (reliableRadio.recvfromAckTimeout(ackResponse, &len, 6000))
    {
      char respByte = (char)ackResponse[0];
      Serial.println("rbyte");
      Serial.println(respByte);
      if (respByte == ACK[0])
      {
        Serial.println("ACKED");
        return TxRx::Delivered;
      }
      else
      {
        Serial.println("NOT ACKED");
        return TxRx::Sent;
      }
    }
    else
    {
      Serial.println("Timeout out waiting for ack");
      return TxRx::Sent;
    }
  }
  else
  {
   Serial.println("Could not confirm send");
  return TxRx::Broadcasted; 
  }
}

TxRx::ConfirmationType TxRx::transmit(char *txMsg)
{
  int retries = 0;
  while (retries <= GLOBAL_TX_RETRIES)
  {
    ConfirmationType txResult = _transmit(txMsg);
    if (txResult == ConfirmationType::Broadcasted)
    {
      retries++;
    }
    else
    {
      return txResult;
      break;
    }
  }
  return ConfirmationType::Broadcasted;
}

char *TxRx::getReceiveMsg()
{
  return rxMsg;
}

bool TxRx::tryReceive()
{
  uint8_t receive_buffer[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(receive_buffer);
  if (reliableRadio.available())
  {
    if (reliableRadio.recvfromAck(receive_buffer, &buflen))
    {
      Serial.println("found msg");
      char syn = (char)receive_buffer[0];
      if (syn != SYN_CHAR)
      {
        Serial.println("could not decrypt");
        bool nacked = reliableRadio.sendtoWait((uint8_t *)NAK, strlen(NAK), DEFAULT_ADDR);
        if (!nacked)
        {
          Serial.println("failed to NAK");
        }
        return false;
      }
      // ACK the message
      Serial.println("sending ack");
      bool acked = reliableRadio.sendtoWait((uint8_t *)ACK, strlen(ACK), DEFAULT_ADDR);
      if (!acked)
      {
        Serial.println("failed to ACK");
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
      return true;
    }
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
  if (!reliableRadio.init())
    Serial.println("init failed");
  if (!radio.setChannel(75))
    Serial.println("setChannel failed");
  if (!radio.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
  reliableRadio.setTimeout(150);
  reliableRadio.setRetries(15);
  // Try to receive to clear out the buffer
  tryReceive();
  radio.setModeRx();
}
