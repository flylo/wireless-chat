// nrf24_server

#include <SPI.h>
#include <RH_NRF24.h>
#include <RHReliableDatagram.h>
// Define addresses for radio channels
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define DEFAULT_ADDR 0

// Singleton instance of the radio driver
RH_NRF24 lol;
// Sets the radio driver to NRF24 and the server address to 2
RHReliableDatagram nrf24(lol, DEFAULT_ADDR);
// RH_NRF24 nrf24(8, 7); // use this to be electrically compatible with Mirf
// RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
// RH_NRF24 nrf24(8, 7); // For RFM73 on Anarduino Mini

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ; // wait for serial port to connect. Needed for Leonardo only
    if (!nrf24.init())
        Serial.println("init failed");
    // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
    //   if (!nrf24.setChannel(1))
    //     Serial.println("setChannel failed");
    //   if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    //     Serial.println("setRF failed");
}

void loop()
{
    if (nrf24.available())
    {
        // Should be a message for us now
        uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        uint8_t from;
        if (nrf24.recvfromAck(buf, &len, &from))
        {
            Serial.print("got request: ");
            Serial.println((char *)buf);

            // Send a reply
            uint8_t data[] = "And hello back to you";
            if (nrf24.sendtoWait(data, sizeof(data), from))
            {
                Serial.println("Sent a reply");
            }
            else
            {
                Serial.println("sendToWait failed");
            }
            
        }
        else
        {
            Serial.println("recv failed");
        }
    }
}
