#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H
#include <Arduino.h>
#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

class DisplayInterface
{

// TODO: needs way of interacting with keyboard (appendChar, deleteChar, etc)
public:
    DisplayInterface();
    void displayFullMessage(char *msg);
    void displayFullMessage(String msg);
    void init();
};

#endif
