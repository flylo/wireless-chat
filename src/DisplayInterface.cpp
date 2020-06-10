#include "DisplayInterface.h"

// NOTE: can't be member variable for some reason
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

DisplayInterface::DisplayInterface()
{
}

void DisplayInterface::displayFullMessage(char *msg)
{
    // There is probably a more efficient way than whiping LCD RAM every epoch
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(msg);
}

void DisplayInterface::displayFullMessage(String msg)
{
    displayFullMessage(msg.begin());
}

void DisplayInterface::init()
{
    lcd.init();
    lcd.backlight();
}
