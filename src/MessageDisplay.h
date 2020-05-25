#ifndef MESSAGE_DISPLAY_H
#define MESSAGE_DISPLAY_H
#include <Arduino.h>
#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

class DisplayInterface
{

public:
    DisplayInterface();
    void displayFullMessage(char *msg);
    void init();
};

#endif
