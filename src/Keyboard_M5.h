#ifndef KEYBOARD_M5_H
#define KEYBOARD_M5_H

#include <Arduino.h>
#include <Wire.h>

#define CARDKB_ADDR 0x5F

class Keyboard_M5
{

public:
    Keyboard_M5();
    void init();
    void loop();
    void clear();
    String get();
};

#endif
