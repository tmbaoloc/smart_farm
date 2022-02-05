#ifndef VOLTAGE_MONITOH_H
#define VOLTAGE_MONITOH_H
#include <EmonLib.h> //Include Emon Library
#include <Wire.h> // LCD
#include <LiquidCrystal_I2C.h> // LCD

class VoltageMonitor
{
public:
    enum State
    {
        NORMAL,
        POWER_LOW,
        POWER_FAILURE
    };
private:
    uint8_t threshold_pin;
    uint8_t button_pin;
    uint8_t buzzer_pin;
    uint8_t current_volt;
    uint8_t threshold_volt;
    volatile uint8_t state;
    volatile bool isStop;
    EnergyMonitor *emon; 
    LiquidCrystal_I2C *lcd;
    
public:
    VoltageMonitor(LiquidCrystal_I2C *_lcd, EnergyMonitor *_emon)
    {
        lcd = _lcd;
        emon = _emon;
        state = NORMAL;
        isStop = false;
    };

    VoltageMonitor()
    {
    };

    ~VoltageMonitor()
    {
    };

    VoltageMonitor* GetInstance()
    {
        static VoltageMonitor *instance;
        return instance;
    };

    uint8_t Monitor();
    void SetPins(uint8_t _threshold_pin, uint8_t _buzzer_pin, uint8_t _button_pin)
    {   
        threshold_pin = _threshold_pin;
        buzzer_pin = _buzzer_pin;
        button_pin = _button_pin;
        
        pinMode(buzzer_pin, OUTPUT);
        pinMode(button_pin, INPUT_PULLUP);
        digitalWrite(buzzer_pin, LOW);
        
        // attachInterrupt(digitalPinToInterrupt(button_pin), this->ButtonISR, FALLING);
    }

    void EnablePowerLow()
    {
        digitalWrite(buzzer_pin,HIGH);
    }

    void DisablePowerLow()
    {
        digitalWrite(buzzer_pin,LOW);
    }

    void SetWarning(bool _isWarning)
    {
        state = _isWarning ? POWER_LOW : POWER_FAILURE;
    }

private:
    void CalculateVoltage();
    void GetVoltage(uint8_t *_curent_voltage, uint8_t *_threshold_voltage)
    {
        *_curent_voltage = current_volt;
        *_threshold_voltage = threshold_volt;
    };
    void PrintVoltage();
    void StopDisplay();
};

#endif // 