#include "voltage_monitor.h"

uint8_t VoltageMonitor::Monitor()
{
    CalculateVoltage();
    PrintVoltage();
    return state;
}
void VoltageMonitor::PrintVoltage()
{
    lcd->clear();

    // First line of LCD
    lcd->setCursor (0,0);
    lcd->print("VAC:");
    lcd->setCursor (5,0);
    lcd->print(current_volt,1);
    lcd->setCursor(8,0);
    lcd->print("V");

    // Second line of LCD
    lcd->setCursor (0,1);
    lcd->print("SET: ");
    lcd->setCursor (5,1);
    lcd->print(threshold_volt);
    lcd->setCursor (8,1);
    lcd->print("V");

    if (state == POWER_LOW)
    {
        lcd->setCursor(10,0);
        lcd->print("[WARN]");
    }
    else
    {
        lcd->setCursor(10,0);
        lcd->print("      ");    
    }
}

void VoltageMonitor::CalculateVoltage()
{
    int ret;
    emon->calcVI(30,3000);               //Calculate all. No.of half wavelengths (crossings), time-out
    current_volt = (int)emon->Vrms;     //extract Vrms into Variable
    threshold_volt = map(analogRead(threshold_pin),0,1023,0,240);

    if (current_volt < 50)
    {
        state = POWER_FAILURE;
    }
    if (current_volt < threshold_volt)
    {
        state = POWER_LOW;
    }
    else
    {
        state = NORMAL;
    }
    // return ret;
}
