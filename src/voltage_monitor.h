#ifndef VOLTAGE_MONITOH_H
#define VOLTAGE_MONITOH_H
#include <EmonLib.h>           //Include Emon Library
#include <Wire.h>              // LCD
#include <LiquidCrystal_I2C.h> // LCD
#include <SoftwareSerial.h>

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
    VoltageMonitor()
    {
        lcd = nullptr;
        emon = new EnergyMonitor();
        state = NORMAL;
        isStop = false;
    };

    ~VoltageMonitor(){};

    uint8_t Monitor();

    void EnablePowerLow();
    void DisablePowerLow();

    void SetPins(uint8_t _threshold_pin, uint8_t _buzzer_pin, uint8_t _button_pin);
    void SetWarning(bool _isWarning);
    void SetupVoltage(int8_t input_pin, float volt_cal, float phase_shift);
    void SetupDisplay(uint8_t lcd_addr, uint8_t lcd_width, uint8_t lcd_height);

    void GetVoltage(uint8_t *_curent_voltage, uint8_t *_threshold_voltage);
private:
    void CalculateVoltage();
    void PrintVoltage();
    void StopDisplay();
};

#endif /* VOLTAGE_MONITOH_H */