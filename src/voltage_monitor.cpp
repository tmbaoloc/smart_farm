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
    lcd->setCursor(0, 0);
    lcd->print("VAC:");
    lcd->setCursor(5, 0);
    lcd->print(current_volt, 1);
    lcd->setCursor(8, 0);
    lcd->print("V");

    // Second line of LCD
    lcd->setCursor(0, 1);
    lcd->print("SET: ");
    lcd->setCursor(5, 1);
    lcd->print(threshold_volt);
    lcd->setCursor(8, 1);
    lcd->print("V");

    if (state == POWER_LOW)
    {
        lcd->setCursor(10, 0);
        lcd->print("[WARN]");
    }
    else
    {
        lcd->setCursor(10, 0);
        lcd->print("      ");
    }
}

void VoltageMonitor::CalculateVoltage()
{
    emon->calcVI(30, 3000); // Calculate all. No.of half wavelengths (crossings), time-out
    current_volt = (int)emon->Vrms;
    threshold_volt = map(analogRead(threshold_pin), 0, 1023, 0, 240);

    if (current_volt < 50)
    {
        state = POWER_FAILURE;
    }
    if (current_volt < threshold_volt)
    {
        Serial.println("warning");
        state = POWER_LOW;
    }
    else
    {
        state = NORMAL;
    }
}

void VoltageMonitor::SetPins(uint8_t _threshold_pin, uint8_t _buzzer_pin, uint8_t _button_pin)
{
    threshold_pin = _threshold_pin;
    buzzer_pin = _buzzer_pin;
    button_pin = _button_pin;

    pinMode(buzzer_pin, OUTPUT);
    pinMode(button_pin, INPUT_PULLUP);
    digitalWrite(buzzer_pin, LOW);
}

void VoltageMonitor::SetupDisplay(uint8_t lcd_addr, uint8_t lcd_width, uint8_t lcd_height)
{
    lcd = new LiquidCrystal_I2C(lcd_addr, lcd_width, lcd_height);
    lcd->init();
    lcd->begin(lcd_width, lcd_height);
    lcd->backlight();
}

void VoltageMonitor::EnablePowerLow()
{
    digitalWrite(buzzer_pin, HIGH);
}

void VoltageMonitor::DisablePowerLow()
{
    digitalWrite(buzzer_pin, LOW);
}

void VoltageMonitor::SetWarning(bool _isWarning)
{
    state = _isWarning ? POWER_LOW : POWER_FAILURE;
}

void VoltageMonitor::SetupVoltage(int8_t input_pin, float volt_cal, float phase_shift)
{
    emon->voltage(input_pin, volt_cal, phase_shift);
}

void VoltageMonitor::GetVoltage(uint8_t *_curent_voltage, uint8_t *_threshold_voltage)
{
    *_curent_voltage = current_volt;
    *_threshold_voltage = threshold_volt;
};