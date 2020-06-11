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

// TODO; don't need to keep
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

int freeMemory()
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

// TODO: break this out - shouldn't need defined loops here
void keyboardLoop()
{
  int start = millis();
  Serial.println(F("trying to handle keyboard input"));
  keyboardM5.loop();
  int end = millis();
  Serial.println("Keyboard MS:");
  Serial.println(end-start);
}

// TODO; lots of leaky abstractions
void displayLoop()
{
  int start = millis();
  Serial.println(F("trying to display"));
  Serial.println(freeMemory());
  String displayMsg = keyboardM5.get();
  // String& ref = displayMsg;
  // Serial.println(ref);
  if (displayMsg.charAt(displayMsg.length() - 1) == '\x1B')
  {
    txRx.transmit(displayMsg.begin());
    keyboardM5.clear();
    displayInterface.displayIncrementalMessage(F("Sent!!"));
    delay(1000);
  }
  displayInterface.displayIncrementalMessage(displayMsg);
  int end = millis();
  Serial.println("Display MS:");
  Serial.println(end-start);
}

void receiveLoop()
{
  int start = millis();
  Serial.println(F("trying to receive"));
  if (txRx.tryReceive())
  {
    Serial.println(F("Received!"));
    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
    Serial.println(txRx.getReceiveMsg());
    displayInterface.displayWithClear(txRx.getReceiveMsg());
  }
  int end = millis();
  Serial.println("Receive MS:");
  Serial.println(end-start);
}

TimedAction keyboardLoopAction = TimedAction(200, keyboardLoop);
TimedAction displayLoopAction = TimedAction(200, displayLoop);
TimedAction receiveLoopAction = TimedAction(200, receiveLoop);

void setup()
{
  Serial.begin(9600);
  keyboardM5.init();
  txRx.init();
  displayInterface.init();
  // TODO; OO setup for buzzer
  pinMode(buzzer, OUTPUT);
  Serial.println("Free Memory Start:");
  Serial.println(freeMemory());
}

int i = 0;

//  TODO: Keyboard firmware seems to store variables
void loop()
{
  keyboardLoopAction.check();
  displayLoopAction.check();
  receiveLoopAction.check();
  Serial.println("Free Memory Loop:");
  Serial.println(i);
  Serial.println(freeMemory());
  i++;
}
