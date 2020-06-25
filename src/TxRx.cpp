#include "TxRx.h"

const char *SYN = "\26\0";
const char *ACK = "\6\0";
const char *NAK = "\25\0";
char rxMsg[RH_NRF24_MAX_MESSAGE_LEN];
RH_NRF24 radio;
AES128 aes = AES128();
RHEncryptedDriver encryptedDriver(radio, aes);

TxRx::TxRx()
{
}

// client: SYN
// server: ACK or NAK
// client: if ACK: send msg
// server: ACK or NAK
// client: if ACK: OK
// client: if NAK: Not OK
// client: if no response: retry
// client: if still no response: timeout
bool serverHandshake()
{
  uint8_t handshakeBuffer[RH_NRF24_MAX_MESSAGE_LEN]; // needs enough space for the headers
  uint8_t buflen = sizeof(handshakeBuffer);
  if (encryptedDriver.recv(handshakeBuffer, &buflen))
  {
    char synByte = (char)handshakeBuffer[0];
    Serial.println("synBytes");
    Serial.println(synByte);
    if (synByte == SYN[0])
    {
      Serial.println("FOUND SYN!");
      encryptedDriver.send((uint8_t *)ACK, strlen(ACK));
      encryptedDriver.waitPacketSent();
      encryptedDriver.send((uint8_t *)ACK, strlen(ACK));
      encryptedDriver.waitPacketSent();
      return true;
    }
    else
    {
      Serial.println("NOT SYN!");
      encryptedDriver.send(NAK, strlen(NAK));
      encryptedDriver.waitPacketSent();
      encryptedDriver.send(NAK, strlen(NAK));
      encryptedDriver.waitPacketSent();
      return false;
    }
  }
  return false;
}

bool clientHandshake()
{
  encryptedDriver.send(SYN, strlen(SYN));
  encryptedDriver.waitPacketSent();
  if (!encryptedDriver.waitAvailableTimeout(2000))
  {
    Serial.println("timeout");
    return false;
  };
  uint8_t response[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(response);
  if (encryptedDriver.recv(response, &len))
  {
    char respByte = (char)response[0];
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
  return false;
}

bool TxRx::transmit(char *txMsg)
{
  if (clientHandshake())
  {
    encryptedDriver.setMode(RHModeTx);
    encryptedDriver.send((uint8_t *)txMsg, strlen(txMsg));
    bool sent = encryptedDriver.waitPacketSent();
    // send twice lol
    delay(100);
    encryptedDriver.send((uint8_t *)txMsg, strlen(txMsg));
    sent = encryptedDriver.waitPacketSent();
    if (sent)
    {
      if (!encryptedDriver.waitAvailableTimeout(2000))
      {
        Serial.println("timeout waiting for receive ack");
        return false;
      };
      uint8_t response[RH_NRF24_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(response);
      if (encryptedDriver.recv(response, &len))
      {
        char respByte = (char)response[0];
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
  }
  return false;
}

char *TxRx::getReceiveMsg()
{
  return rxMsg;
}

bool TxRx::tryReceive()
{
  if (serverHandshake())
  {
    if (!encryptedDriver.waitAvailableTimeout(2000))
    {
      Serial.println("receive timeout");
      return false;
    };
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
      // ACK the message
      // NOTE: seems like ACKing twice works better... TX->RX switching is slow i guess
      delay(100); // pause to allow for client to switch back to RX mode
      encryptedDriver.send((uint8_t *)ACK, strlen(ACK));
      encryptedDriver.waitPacketSent();
      delay(100); // pause to allow for client to switch back to RX mode
      encryptedDriver.send((uint8_t *)ACK, strlen(ACK));
      encryptedDriver.waitPacketSent();
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
  if (!encryptedDriver.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!radio.setChannel(1))
    Serial.println("setChannel failed");
  if (!radio.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
  // Try to receive to clear out the buffer
  tryReceive();
}
