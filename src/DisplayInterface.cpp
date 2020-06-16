#include "DisplayInterface.h"

// NOTE: can't be member variable for some reason
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);


DisplayInterface::DisplayInterface()
{
    lastPointer = 0;
}

// TODO: clean all the bullshit prints
void DisplayInterface::displayIncrementalMessage(char *msg, int pointer)
{
    // There is probably a more efficient way than whiping LCD RAM every epoch
    Serial.println(F("Displaying"));
    Serial.println(msg);
    lcd.clear();
    lcd.print(msg);
    // if (pointer == 0 && lastPointer > 0)
    // {
    //     Serial.println(F("Triggered: "));
    //     Serial.println(F("1"));
    //     lastPointer = 0;
    //     lcd.clear();
    //     return;
    // }
    // if (pointer > 0 && lastPointer != pointer)
    // {
    //     Serial.println(F("Triggered: "));
    //     Serial.println(F("2"));
    //     lcd.print(msg[pointer]);
    //     lastPointer = pointer;
    //     return;
    // }
    // if (pointer < lastDisplayed.length()) {
    //     Serial.println(F("Triggered: "));
    //     Serial.println(F("3"));
    //     lcd.clear();
    //     if (newMsg.length() > 0)
    //     {
    //         lcd.print(newMsg);
    //         lastDisplayed = newMsg;
    //     } else
    //     {
    //         lastDisplayed = "";
    //     }
    // }
}

void DisplayInterface::displayIncrementalMessage(String msg)
{
    displayIncrementalMessage(msg.begin());
}

void DisplayInterface::displayWithClear(String msg)
{
    lcd.clear();
    lcd.print(msg);
    // TODO: return to previous buffer once we've read message
    // delay(5000);
    // lcd.clear();
    // lcd.print(lastDisplayed);
}

void DisplayInterface::init()
{
    lcd.init();
    lcd.clear();
    lcd.backlight();
}
