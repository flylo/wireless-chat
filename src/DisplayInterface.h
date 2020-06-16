#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H
#include <Arduino.h>
#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

class DisplayInterface
{

private:
  int lastPointer;

// TODO: needs way of interacting with keyboard (appendChar, deleteChar, etc)
public:
    DisplayInterface();
    void displayIncrementalMessage(char *msg, int pointer);
    void displayIncrementalMessage(String msg);
    void displayWithClear(String msg);
    void init();
};

#endif
