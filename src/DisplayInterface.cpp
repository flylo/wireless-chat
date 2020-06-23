#include "DisplayInterface.h"

// NOTE: can't be member variable for some reason
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
char msgLine0[17];
char msgLine1[17];
int pointer = 0;

DisplayInterface::DisplayInterface()
{
}

void DisplayInterface::displayMsg(char *msg)
{
    // There is probably a more efficient way than whiping LCD RAM every epoch
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(msg);
    for (int i = 0; i < 16; i++)
    {
        msgLine0[i] = msg[i];
        msgLine1[i] = msg[i + 16];
    }
    lcd.setCursor(0, 0);
    lcd.print(msgLine0);
    lcd.setCursor(0, 1);
    lcd.print(msgLine1);
    for (int i = 0; i < 16; i++)
    {
        msgLine0[i] = '\0';
        msgLine1[i] = '\0';
    }
}

void DisplayInterface::init()
{
    lcd.init();
    lcd.backlight();
    Serial.println("lcdinit");
}
