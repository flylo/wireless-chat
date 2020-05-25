#include "DisplayInterface.h"

// NOTE: can't be member variable for some reason
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

DisplayInterface::DisplayInterface()
{
}

void DisplayInterface::displayFullMessage(char *msg)
{
    lcd.setCursor(0, 0);
    lcd.print("Message Received!!");
    lcd.setCursor(0, 1);
    lcd.print(msg);
    Serial.print("Message: ");
    Serial.println(msg);
}

void DisplayInterface::init()
{
    lcd.init();
    lcd.backlight();
}