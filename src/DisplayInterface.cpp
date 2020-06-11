#include "DisplayInterface.h"

// NOTE: can't be member variable for some reason
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);


// TODO; don't need to keep
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

int freeMemoryMoarMoar()
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



DisplayInterface::DisplayInterface()
{
    lastDisplayed = "";
}

// TODO: clean all the bullshit prints
void DisplayInterface::displayIncrementalMessage(char *msg)
{
    // There is probably a more efficient way than whiping LCD RAM every epoch
    Serial.println(F("Displaying"));
    Serial.println(freeMemoryMoarMoar());
    String newMsg = String(msg);
    Serial.println(newMsg);
    Serial.println(freeMemoryMoarMoar());
    if (newMsg.length() == 0 && lastDisplayed.length() != 0)
    {
        Serial.println(F("Triggered: "));
        Serial.println(F("1"));
        lastDisplayed = "";
        lcd.clear();
        return;
    }
    if (newMsg.length() > 0 && newMsg.startsWith(lastDisplayed))
    {
        Serial.println(F("Triggered: "));
        Serial.println(F("2"));
        lcd.print(newMsg.substring(lastDisplayed.length()));
        lastDisplayed = newMsg;
        return;
    }
    if (newMsg.length() < lastDisplayed.length()) {
        Serial.println(F("Triggered: "));
        Serial.println(F("3"));
        lcd.clear();
        if (newMsg.length() > 0)
        {
            lcd.print(newMsg);
            lastDisplayed = newMsg;
        } else
        {
            lastDisplayed = "";
        }
    }
}

void DisplayInterface::displayIncrementalMessage(String msg)
{
    displayIncrementalMessage(msg.begin());
}

void DisplayInterface::displayWithClear(String msg)
{
    lcd.clear();
    lcd.print(msg);
    delay(5000);
    lcd.clear();
    lcd.print(lastDisplayed);
}

void DisplayInterface::init()
{
    lcd.init();
    lcd.clear();
    lcd.backlight();
}
