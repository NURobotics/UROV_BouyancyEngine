#include <Arduino.h>
#include "BluetoothSerial.h"
#include <DS3231.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "1e5fbc5e-3f2d-4edd-bf9a-bce8ac61cb62"
#define TEAM_CHARACTERISTIC_UUID "679b8753-ef29-44a2-913b-260cf7cb9cab"
#define TIME_CHARACTERISTIC_UUID "8a1cfa2d-323c-45a4-8fda-dc65a1834825"

class BTTimeTransmitter
{
public:
    BTTimeTransmitter(){};

    void Init(int sda, int scl, uint32_t new_period = 200)
    {
        // serial_bt_.begin("UROV_BuoyancyEngine"); // Bluetooth device name
        BLEDevice::init("UROV Buoyancy Engine");
        BLEServer *pServer = BLEDevice::createServer();
        BLEService *pService = pServer->createService(SERVICE_UUID);
        pTeamCharacteristic = pService->createCharacteristic(
            TEAM_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE);
        pTimeCharacteristic = pService->createCharacteristic(
            TIME_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE);

        pTeamCharacteristic->setValue("Northwestern UROV");

        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();

        // Serial.println("The device started, now you can pair it with bluetooth!");
        Wire.begin(sda, scl);
        period_ = new_period;
        last_transmit_ = millis() - period_;
    }

    void Tick()
    {
        if (millis() - period_ >= last_transmit_)
        {
            Transmit();
            last_transmit_ = millis();
        }
    }

private:
    DS3231 my_clock_;
    bool century_ = false;
    bool h12Flag_;
    bool pmFlag_;

    // BluetoothSerial serial_bt_;
    BLECharacteristic *pTeamCharacteristic;
    BLECharacteristic *pTimeCharacteristic;

    String to_send;

    uint32_t period_;
    uint32_t last_transmit_;

    void Transmit()
    {
        to_send = ""; //"Northwestern ";
        to_send += my_clock_.getYear();
        to_send += "-";
        to_send += my_clock_.getMonth(century_);
        to_send += "-";
        to_send += my_clock_.getDate();
        to_send += " ";
        to_send += my_clock_.getHour(h12Flag_, pmFlag_);
        to_send += ":";
        to_send += my_clock_.getMinute();
        to_send += ":";
        to_send += my_clock_.getSecond();
        to_send += '\n';
        // serial_bt_.print(to_send);
        pTimeCharacteristic->setValue(to_send.c_str());
        /* Serial.print(my_clock_.getYear(), DEC);
        Serial.print("-");
        Serial.print(my_clock_.getMonth(century_), DEC);
        Serial.print("-");
        Serial.print(my_clock_.getDate(), DEC);
        Serial.print(" ");
        Serial.print(my_clock_.getHour(h12Flag_, pmFlag_), DEC); // 24-hr
        Serial.print(":");
        Serial.print(my_clock_.getMinute(), DEC);
        Serial.print(":");
        Serial.println(my_clock_.getSecond(), DEC); */
    }
};