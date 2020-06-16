#include <Keyboard_M5.h>

const int bufSize = 12;
char keyboardMsg[bufSize];
int pointer = 0;
const int DEL = 8;
const int RETURN = 13;
const char NULL_CHAR = '\0';
const char ESC_CHAR = '\x1B';

// TODO: initialize with a max buffer length
Keyboard_M5::Keyboard_M5()
{
    // msg = "";
}

void Keyboard_M5::init()
{
    Serial.println("initializing keyboard driver");
    Serial.println(keyboardMsg);
    Serial.println(get());
    Wire.begin();
    //  Clear out anything on the wire
    Wire.requestFrom(CARDKB_ADDR, 1);
    while (Wire.available())
    {
        Wire.read();
    }
}

// TODO; keep adding letters until stop signal, then add null terminator and send
void Keyboard_M5::loop()
{
    Serial.println("Beginning of Keyboard_M5::loop");
    Serial.println("msg:");
    Serial.println(keyboardMsg);
    Serial.println(F("Trying to request from wire"));
    Wire.requestFrom(CARDKB_ADDR, 1);
    while (Wire.available())
    {
        Serial.println(F("Trying to read from wire"));
        char c = Wire.read(); // receive a byte as characterif
        Serial.println("found char");
        Serial.println(c);
        Serial.println(int(c));
        Serial.println("msg:");
        Serial.println(keyboardMsg);
        switch (int(c))
        {
        case 0:
            // do nothing
            Serial.println("Triggered 0");
            Serial.println("msg:");
            Serial.println(keyboardMsg);
            break;
        case DEL:
            if (pointer > 0)
            {
                int i = pointer;
                while (i < bufSize)
                {
                    keyboardMsg[i] = NULL_CHAR;
                    i++;
                }
            }
            Serial.println("Triggered 1");
            Serial.println("msg:");
            Serial.println(keyboardMsg);
            pointer--;
            break;
        case RETURN:
            keyboardMsg[pointer] = ESC_CHAR;
            Serial.println("Triggered 2");
            Serial.println("msg:");
            Serial.println(keyboardMsg);
            break;
        default:
            Serial.println(F("Adding char"));
            Serial.println(keyboardMsg);
            Serial.println(c);
            Serial.println("moar");
            keyboardMsg[pointer] = c;
            keyboardMsg[pointer+1] = NULL_CHAR;
            pointer++;
            Serial.println("Updated message");
            Serial.println("Triggered 3");
            Serial.println("msg:");
            Serial.println(keyboardMsg);
            break;
        }
        Serial.println("reached end of keyboard loop");
    }
}

void Keyboard_M5::clear()
{
    int i = 0;
    while (i < bufSize)
    {
        keyboardMsg[i] = NULL_CHAR;
        i++;
    }
    pointer = 0;
}

char *Keyboard_M5::get()
{
    Serial.println("inside of get");
    Serial.println(keyboardMsg);
    return keyboardMsg;
}

bool Keyboard_M5::escaped()
{
    return keyboardMsg[pointer] == ESC_CHAR;
}
