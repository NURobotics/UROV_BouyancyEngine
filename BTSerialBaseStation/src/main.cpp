#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// String MACadd = "AA:BB:CC:11:22:33";
String name = "ESP32test";
// uint8_t address[6] = {0x80, 0x08, 0x1E, 0x54, 0x20, 0x69};
// uint8_t address[6] = {0x5B, 0xBF, 0x25, 0x83, 0x0B, 0x76};
// char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

void setup()
{
  Serial.begin(9600);
  // SerialBT.setPin(pin);
  SerialBT.begin("UROV_BuoyancyEngine_BaseStation", true);
  // SerialBT.setPin(pin);
  Serial.println("The device started in master mode, make sure remote BT device is on!");

  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  connected = SerialBT.connect(name);
  // connected = SerialBT.connect(address);

  if (connected)
  {
    Serial.println("Connected Succesfully!");
  }
  else
  {
    while (!SerialBT.connected(1000))
    {
      SerialBT.connect();
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    }
  }
  // disconnect() may take upto 10 secs max
  /*  if (SerialBT.disconnect())
   {
     Serial.println("Disconnected Succesfully!");
   } */
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  // SerialBT.connect();
}

void loop()
{
  while (!SerialBT.connected(1000))
  {
    SerialBT.connect();
    Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
  }
  if (Serial.available())
  {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available())
  {
    Serial.write(SerialBT.read());
  }
  delay(20);
}