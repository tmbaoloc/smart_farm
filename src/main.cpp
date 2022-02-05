#include "Arduino.h"
#include "voltage_monitor.h"
#include <Wire.h>              // LCD
#include <LiquidCrystal_I2C.h> // LCD
#include "EmonLib.h"           //Include Emon Library

/* LCD definitions */
#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define LCD_ADDRESS 0x27

/* Voltage sensor definitions */
#define VOLT_CAL 258.7
#define VOLT_PIN 0
#define VOLT_PHASE_SHIFT 1.7

/* Pin definitions */
#define PIN_THRESHOLD 15
#define PIN_BUZZER 9
#define PIN_BUTTON 2

volatile bool warning = false;
LiquidCrystal_I2C *lcd = nullptr;
EnergyMonitor *EM = nullptr;
VoltageMonitor *VM = nullptr;

void do_warning()
{
}

void button_ISR()
{
    VM->SetWarning(false);
    digitalWrite(PIN_BUZZER, LOW);
    Serial.println("Warning stop");
}

void setup()
{
    Serial.begin(9600);

    VM = new VoltageMonitor();
    VM->SetupDisplay(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
    VM->SetupVoltage(VOLT_PIN, VOLT_CAL, VOLT_PHASE_SHIFT);
    VM->SetPins(PIN_THRESHOLD, PIN_BUZZER, PIN_BUTTON);

    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), button_ISR, FALLING);
}

void loop()
{
    static int ret = 0;
    ret = VM->Monitor();
    if (ret == VoltageMonitor::POWER_LOW)
    {
        VM->EnablePowerLow();
    }
    else if (ret == VoltageMonitor::NORMAL)
    {
        VM->DisablePowerLow();
    }
    else if (ret == VoltageMonitor::POWER_FAILURE)
    {
    }

    delay(500);
}
