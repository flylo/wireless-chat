#include <Keyboard_M5.h>

String msg;
const int DEL = 8;
const int RETURN = 13;
const char NULL_CHAR = '\0';
const char ESC_CHAR = '\x1B';

// TODO; don't need to keep
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

int freeMemoryMoar()
{
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
}
// TODO: smoke

// TODO: initialize with a max buffer length
Keyboard_M5::Keyboard_M5()
{
    msg = "";
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
}

// TODO; keep adding letters until stop signal, then add null terminator and send
void Keyboard_M5::loop()
{
    Serial.println(F("Trying to request from wire"));
    Serial.println(freeMemoryMoar());
    Wire.requestFrom(CARDKB_ADDR, 1);
    while (Wire.available())
    {
        Serial.println(F("Trying to read from wire"));
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
            Serial.println(F("Adding char"));
            Serial.println(freeMemoryMoar());
            Serial.println(msg);
            Serial.println(c);
            Serial.println("moar");
            msg += c;
            Serial.println("Updated message");
            break;
        }
        Serial.println("reached end of keyboard loop");
    }
}

void Keyboard_M5::clear()
{
    msg = "";
}

String Keyboard_M5::get()
{
    Serial.println(F("msg:"));
    Serial.println(freeMemoryMoar());
    Serial.println(msg);
    return msg;
}
