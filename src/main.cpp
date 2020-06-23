#include <Arduino.h>
// #include <TxRx.h>
#include <DisplayInterface.h>
#include <Keyboard_M5.h>
#include <TimedAction.h>
#include <MemoryFree.h>

// const int buzzer = 9;
// TxRx txRx = TxRx(2000, 11, 12);
DisplayInterface displayInterface = DisplayInterface();
Keyboard_M5 keyboardM5 = Keyboard_M5();


// TODO: break this out - shouldn't need defined loops here
void keyboardLoop()
{
  int start = millis();
  // Serial.println(F("trying to handle keyboard input"));
  keyboardM5.loop();
  int end = millis();
  Serial.println("Keyboard MS:");
  Serial.println(end - start);
}

// TODO; lots of leaky abstractions. move things to a display::loop()
void displayLoop()
{
  int start = millis();
  Serial.println(F("trying to display"));
  Serial.println(keyboardM5.get());
  displayInterface.displayMsg(keyboardM5.get());
  if (keyboardM5.escaped())
  {
    Serial.println("clearing");
    // txRx.transmit(keyboardM5.get());
    keyboardM5.clear();
    // displayInterface.displayIncrementalMessage(F("Sent!!"));
    delay(1000);
  }
  // displayInterface.displayIncrementalMessage(keyboardM5.get());
  int end = millis();
  Serial.println("Display MS:");
  Serial.println(end - start);
}

// void receiveLoop()
// {
//   int start = millis();
//   Serial.println(F("trying to receive"));
//   if (txRx.tryReceive())
//   {
//     Serial.println(F("Received!"));
//     tone(buzzer, 1000);
//     delay(300);
//     noTone(buzzer);
//     Serial.println(txRx.getReceiveMsg());
//     displayInterface.displayWithClear(txRx.getReceiveMsg());
//   }
//   int end = millis();
//   Serial.println("Receive MS:");
//   Serial.println(end - start);
// }

TimedAction keyboardLoopAction = TimedAction(100, keyboardLoop);
TimedAction displayLoopAction = TimedAction(100, displayLoop);
// TimedAction receiveLoopAction = TimedAction(100, receiveLoop);

void setup()
{
  Serial.begin(9600);
  Serial.println("initializing");
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  displayInterface.init();
  keyboardM5.init();
  // txRx.init();
  // TODO; OO setup for buzzer
  // pinMode(buzzer, OUTPUT);
}

//  TODO: Keyboard firmware seems to store variables
void loop()
{
  Serial.println(F("main loop:"));
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  keyboardLoopAction.check();
  displayLoopAction.check();
  // receiveLoopAction.check();
}
