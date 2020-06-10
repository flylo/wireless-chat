#include <Keyboard_M5.h>

String msg;
const int DEL = 8;
const int RETURN = 13;
const char NULL_CHAR = '\0';
const char ESC_CHAR = '\x1B';

// TODO: initialize with a max buffer length
Keyboard_M5::Keyboard_M5()
{
    msg = "";
}

void Keyboard_M5::init()
{
    Wire.begin();
}

// TODO; keep adding letters until stop signal, then add null terminator and send
void Keyboard_M5::loop()
{
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
            if (msg.length() > 0)
            {
                msg = msg.substring(0, msg.length() - 1);
            }
            break;
        case RETURN:
            msg = msg + ESC_CHAR;
            break;
        default:
            msg = msg + c;
            break;
        }
    }
}

void Keyboard_M5::clear()
{
    msg = "";
}

String Keyboard_M5::get()
{
    return msg;
}
