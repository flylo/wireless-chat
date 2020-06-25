#include <Keyboard_M5.h>

const int BUF_SIZE = 30;
const int DEL = 8;
const int RETURN = 13;
const char NULL_CHAR = '\0';
const char ESC_CHAR = '\x1B';

// TODO: initialize with a max buffer length
Keyboard_M5::Keyboard_M5()
{
    keyboardMsgBuffer = new char[BUF_SIZE];
    pointer = 0;
}

void Keyboard_M5::init()
{
    Wire.begin();
    //  Clear out anything on the wire
    Wire.requestFrom(CARDKB_ADDR, 1);
    while (Wire.available())
    {
        Wire.read();
    }
    // Clear out the buffer
    for (int i = 0; i < BUF_SIZE; i++)
    {
        keyboardMsgBuffer[i] = NULL_CHAR;
    }
}

void Keyboard_M5::loop()
{
    Wire.requestFrom(CARDKB_ADDR, 1);
    while (Wire.available())
    {
        char c = Wire.read();
        switch (int(c))
        {
        case 0:
            // do nothing
            break;
        case DEL:
            if (pointer > 0)
            {
                keyboardMsgBuffer[pointer - 1] = NULL_CHAR;
            }
            pointer--;
            break;
        case RETURN:
            keyboardMsgBuffer[pointer] = ESC_CHAR;
            break;
        default:
            if (pointer < BUF_SIZE - 1)
            {
                Serial.println(c);
                keyboardMsgBuffer[pointer] = c;
                pointer++;
                Serial.println(keyboardMsgBuffer);
            }
            break;
        }
    }
}

void Keyboard_M5::clear()
{
    int i = 0;
    while (i < BUF_SIZE)
    {
        keyboardMsgBuffer[i] = NULL_CHAR;
        i++;
    }
    pointer = 0;
}

char *Keyboard_M5::get()
{
    return keyboardMsgBuffer;
}

bool Keyboard_M5::escaped()
{
    return keyboardMsgBuffer[pointer] == ESC_CHAR;
}
