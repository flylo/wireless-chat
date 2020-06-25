#include <Arduino.h>
#include <TxRx.h>
#include <DisplayInterface.h>
#include <Keyboard_M5.h>
#include <PiezoBuzzer.h>
#include <TimedAction.h>
#include <MemoryFree.h>

const uint8_t PIN_SIZE = 16;
char PIN[PIN_SIZE];
TxRx txRx = TxRx(*PIN);
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

void enterPin()
{
  displayInterface.displayMsg("Wireless Chat Appliance");
  delay(2000);
  char pinMsg[32];
  char *promptMsg = "Enter PIN:";
  while (!keyboardM5.escaped())
  {
    uint8_t i = 0;
    while (i < 16)
    {
      if (i < strlen(promptMsg))
      {
        pinMsg[i] = promptMsg[i];
      }
      else
      {
        pinMsg[i] = '\0';
      }
      i++;
    }
    keyboardM5.loop();
    char *currentPinBuffer = keyboardM5.get();
    uint8_t j = 0;
    while (j < PIN_SIZE)
    {
      pinMsg[j + PIN_SIZE] = currentPinBuffer[j];
      PIN[j] = currentPinBuffer[j];
      j++;
    }
    displayInterface.displayMsg(pinMsg);
    delay(200);
  }
  char *currentPinBuffer = keyboardM5.get();
  int j = 0;
  while (j < PIN_SIZE)
  {
    PIN[j] = currentPinBuffer[j];
    j++;
  }
  keyboardM5.clear();
  displayInterface.displayMsg("Success!\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
  delay(2000);
  displayInterface.displayMsg("Welcome to Chat!");
  delay(2000);
  displayInterface.displayMsg("Using PIN:");
  delay(2000);
  displayInterface.displayMsg(PIN);
  delay(2000);
}

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
  enterPin();
}

void loop()
{
  Serial.println(F("main loop:"));
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  keyboardLoopAction.check();
  displayLoopAction.check();
  receiveLoopAction.check();
}
