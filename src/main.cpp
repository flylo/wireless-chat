#include <Arduino.h>
#include <TxRx.h>
#include <DisplayInterface.h>
#include <Keyboard_M5.h>
#include <Wire.h>
#include <TimedAction.h>

const int buzzer = 9;
TxRx txRx = TxRx(2000, 11, 12);
DisplayInterface displayInterface = DisplayInterface();
Keyboard_M5 keyboardM5 = Keyboard_M5();

// TODO: break this out - shouldn't need defined loops here
void keyboardLoop()
{
  keyboardM5.loop();
}

// TODO; lots of leaky abstractions
void displayLoop()
{
  String displayMsg = keyboardM5.get();
  if (displayMsg.charAt(displayMsg.length() - 1) == '\x1B')
  {
    txRx.transmit(displayMsg.begin());
    keyboardM5.clear();
    displayInterface.displayFullMessage("Sent!!");
    delay(1000);
  }
  displayInterface.displayFullMessage(displayMsg);
}

void receiveLoop()
{
  if (txRx.tryReceive())
  {
    tone(buzzer, 1000);
    delay(100);
    noTone(buzzer);
    displayInterface.displayFullMessage(txRx.getReceiveMsg());
  }
}

TimedAction keyboardLoopAction = TimedAction(50, keyboardLoop);
TimedAction displayLoopAction = TimedAction(50, displayLoop);
TimedAction receiveLoopAction = TimedAction(50, receiveLoop);

void setup()
{
  Serial.begin(9600);
  keyboardM5.init();
  txRx.init();
  displayInterface.init();
  // TODO; OO setup for buzzer
  pinMode(buzzer, OUTPUT);
}

//  TODO: possible memory leak...
void loop()
{
  keyboardLoopAction.check();
  displayLoopAction.check();
  // receiveLoopAction.check();
}
