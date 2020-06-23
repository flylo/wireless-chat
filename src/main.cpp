#include <Arduino.h>
// #include <TxRx.h>
#include <DisplayInterface.h>
#include <Keyboard_M5.h>
#include <TimedAction.h>
#include <MemoryFree.h>
#include <RH_NRF24.h>

const int buzzer = 9;
// TxRx txRx = TxRx(2000, 11, 12);
DisplayInterface displayInterface = DisplayInterface();
Keyboard_M5 keyboardM5 = Keyboard_M5();
RH_NRF24 nrf24;

// TODO: break this out - shouldn't need defined loops here
void keyboardLoop()
{
  int start = millis();
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
    displayInterface.displayMsg(F("Sent!!"));
    delay(1000);
  }
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

  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");

  // txRx.init();
  // TODO; OO setup for buzzer
  pinMode(buzzer, OUTPUT);
}

//  TODO: Keyboard firmware seems to store variables
void loop()
{
  Serial.println(F("main loop:"));
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  keyboardLoopAction.check();
  displayLoopAction.check();
  // TODO smoke
  Serial.println("Sending to nrf24_server");
  // Send a message to nrf24_server
  uint8_t data[] = "Hello World!";
  nrf24.send(data, sizeof(data));

  nrf24.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (nrf24.waitAvailableTimeout(500))
  {
    // Should be a reply message for us now
    if (nrf24.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char *)buf);
      tone(buzzer, 1000);
      delay(300);
      noTone(buzzer);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is nrf24_server running?");
  }
  delay(400);

  // receiveLoopAction.check();
}
