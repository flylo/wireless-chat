#include <Keyboard_M5.h>

const int BUF_SIZE = 8;
const int DEL = 8;
const int RETURN = 13;
const char NULL_CHAR = '\0';
const char ESC_CHAR = '\x1B';

// TODO: initialize with a max buffer length
Keyboard_M5::Keyboard_M5()
{
    keyboardMsg[BUF_SIZE];
    pointer = 0;
}

void Keyboard_M5::init()
{
    Serial.println("initializing keyboard driver");
    Wire.begin();
    //  Clear out anything on the wire
    Wire.requestFrom(CARDKB_ADDR, 1);
    while (Wire.available())
    {
        Wire.read();
    }
    Serial.println("Successfully initialized keyboard driver");
}

// TODO; keep adding letters until stop signal, then add null terminator and send
void Keyboard_M5::loop()
{
    Serial.println("in loop");
    Wire.requestFrom(CARDKB_ADDR, 1);
    while (Wire.available())
    {
        char c = Wire.read(); // receive a byte as characterif
        switch (int(c))
        {
        case 0:
            // do nothing
            break;
        case DEL:
            if (pointer > 0)
            {
                keyboardMsg[pointer - 1] = NULL_CHAR;
            }
            pointer--;
            break;
        case RETURN:
            keyboardMsg[pointer] = ESC_CHAR;
            break;
        default:
            keyboardMsg[pointer] = c;
            pointer++;
            break;
        }
    }
}

void Keyboard_M5::clear()
{
    int i = 0;
    while (i < BUF_SIZE)
    {
        keyboardMsg[i] = NULL_CHAR;
        i++;
    }
    pointer = 0;
}

char *Keyboard_M5::get()
{
    return keyboardMsg;
}

bool Keyboard_M5::escaped()
{
    return keyboardMsg[pointer] == ESC_CHAR;
}
