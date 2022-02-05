#include "Arduino.h"
#include "voltage_monitor.h"
#include <Wire.h> // LCD
#include <LiquidCrystal_I2C.h> // LCD
#include "EmonLib.h" //Include Emon Library

#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define VOLT_CAL 258.7 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO)**258.7 84%
#define THRESHOLD 15
#define BUZZER 9
#define BUTTON 2

volatile bool warning = false;
LiquidCrystal_I2C *lcd = nullptr;
EnergyMonitor *EM = nullptr;
VoltageMonitor* VM = nullptr;

void do_warning()
{
  
}

void button_ISR()
{
    VM->SetWarning(false);
    digitalWrite(BUZZER,LOW);
    Serial.println("Warning stop");
}

void setup()
{  
    Serial.begin(9600);
    
    EM = new EnergyMonitor();
    EM->voltage(0, VOLT_CAL, 1.7); //Voltage: input pin, calibration, phase_shift
    
    lcd = new LiquidCrystal_I2C(0x27, LCD_WIDTH, LCD_HEIGHT); // LCD
    lcd->init();                      // initialize the lcd 
    lcd->begin(LCD_WIDTH, LCD_HEIGHT); // LCD
    lcd->backlight(); // LCD

    VM = new VoltageMonitor(lcd, EM);
    VM->SetPins(THRESHOLD, BUZZER, BUTTON);
    attachInterrupt(digitalPinToInterrupt(BUTTON), button_ISR, FALLING);
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
    Serial.println("Warning");
    delay(500);
}
