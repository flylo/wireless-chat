#include <Arduino.h>
/* I2C LCD with Arduino example code. More info: https://www.makerguides.com */

// Include the libraries:
// LiquidCrystal_I2C.h: https://github.com/johnrickman/LiquidCrystal_I2C
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <RH_ASK.h>
#include <SPI.h>
// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,16,2) for 16x2 LCD.

// TRANSMIT
// Speed of 2000 bits per second
// Use pin 12 for transmission
// RH_ASK radio(2000, 11, 12);

// RECEIVE
RH_ASK radio(2000, 11, 12);

void setup() {
  lcd.init();
  lcd.backlight();

  // TRANSMIT: usbmodem1451
  // Serial.begin(9600);
  // if (!radio.init())
  // {
  //      Serial.println("Radio module failed to initialize");
  // }
  // Serial.println("Radio module initialized successfully!")

  // RECEIVE: usbmodem1431
  Serial.begin(9600);
  if (!radio.init())
  {
       Serial.println("Radio module failed to initialize");
  }
  Serial.println("Radio module initialized successfully!");
}

void loop() {
  // TRANSMIT
  // const char *msg = "Hello World";
  // radio.send((uint8_t*)msg, strlen(msg));
  // radio.waitPacketSent();
  // delay(500);
  // Serial.println("Data Sent");

  // RECEIVE
  uint8_t receive_buffer[32];
  uint8_t buflen = sizeof(receive_buffer);
  if (radio.recv(receive_buffer, &buflen))
  {
    lcd.setCursor(0, 0);
    lcd.print("Message Received!!");
    lcd.setCursor(2, 1);
    lcd.print("lol RADAR sux");
    Serial.print("Message: ");
    Serial.println((char*)receive_buffer);
    delay(1000);
    lcd.clear();
  }

}
