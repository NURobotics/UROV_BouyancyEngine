#include <Arduino.h>
#include "BluetoothSerial.h"
#include <DS3231.h>
#include <Wire.h>

class BTTimeTransmitter
{
public:
    BTTimeTransmitter(){};

    void Init(int sda, int scl, uint32_t new_period = 200)
    {
        serial_bt_.begin("UROV_BuoyancyEngine"); // Bluetooth device name
        Serial.println("The device started, now you can pair it with bluetooth!");
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

    BluetoothSerial serial_bt_;

    uint32_t period_;
    uint32_t last_transmit_;

    void Transmit()
    {
        String to_send = "Northwestern ";
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
        serial_bt_.print(to_send);
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