#include <Arduino.h>
#include <TxRx.h>
#include <DisplayInterface.h>
#include <Keyboard_M5.h>
#include <PiezoBuzzer.h>
#include <TimedAction.h>
#include <MemoryFree.h>

TxRx txRx = TxRx();
DisplayInterface displayInterface = DisplayInterface();
Keyboard_M5 keyboardM5 = Keyboard_M5();
PiezoBuzzer piezoBuzzer = PiezoBuzzer();

void keyboardLoop()
{
  int start = millis();
  keyboardM5.loop();
  int end = millis();
  Serial.println("Keyboard MS:");
  Serial.println(end - start);
}

void displayLoop()
{
  int start = millis();
  Serial.println(F("trying to display"));
  Serial.println(keyboardM5.get());
  displayInterface.displayMsg(keyboardM5.get());
  if (keyboardM5.escaped())
  {
    Serial.println("clearing");
    // TODO: transmit in the display loop makes no sense
    txRx.transmit(keyboardM5.get());
    keyboardM5.clear();
    displayInterface.displayMsg("Sent!");
    delay(1000);
  }
  int end = millis();
  Serial.println("Display MS:");
  Serial.println(end - start);
}

void receiveLoop()
{
  int start = millis();
  Serial.println(F("trying to receive"));
  if (txRx.tryReceive())
  {
    Serial.println(F("Received!"));
    piezoBuzzer.buzz();
    displayInterface.displayMsg((char *)txRx.getReceiveMsg());
    delay(5000);
    txRx.clear();
  }
  int end = millis();
  Serial.println("Receive MS:");
  Serial.println(end - start);
}

TimedAction keyboardLoopAction = TimedAction(100, keyboardLoop);
TimedAction displayLoopAction = TimedAction(300, displayLoop);
TimedAction receiveLoopAction = TimedAction(100, receiveLoop);

void setup()
{
  Serial.begin(9600);
  Serial.println("initializing");
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  displayInterface.init();
  keyboardM5.init();
  txRx.init();
  piezoBuzzer.init();
}

//  TODO: Keyboard firmware seems to store variables
void loop()
{
  Serial.println(F("main loop:"));
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  keyboardLoopAction.check();
  displayLoopAction.check();
  receiveLoopAction.check();
}
