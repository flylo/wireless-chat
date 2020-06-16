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
    lcd.print("LOLMOAR");
    Serial.println("Successfully initialized LCD driver");
}
