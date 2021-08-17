/**
 * @file USBDroneController.ino
 * @author Hobbylad ()
 * @brief Example of ScanADC Library for a USB HID drone controller.
 * @version 0.1
 * @date 2021-07-15
 * @copyright Copyright (c) 2021
 *
 * Simple USB drone controller for PC simulator.
 *
 * Prototyped on 5V Pro Micro ATMega32U4 (Arduino Leonardo) and wired to hacked left & right
 * 2-axis joystick drone controller outputing 0 - 3.3V per axis.
 *
 * Appears at HID standard device and does not need custom driver.
 *
 * Use axis calibration and axis "invert" if necessary in drone simulator controller setup.
 *
 * Compiled with Arduino 1.8.13 IDE and tested on CurryKitten FPV Simulator (PC).
 *
 *
 * MIT License
 *
 * Copyright (c) 2021 Hobbylad
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


//
// Simple USB drone controller for PC simulator.
//
// Prototyped on 5V Pro Micro ATMega32U4 (Arduino Leonardo) and wired to hacked left & right
// 2-axis joystick drone controller outputing 0 - 3.3V per axis.
//
// Appears at HID standard device and does not need custom driver.
//
// Use axis calibration and axis "invert" if necessary in drone simulator controller setup.
//
// Compiled with Arduino 1.8.13 IDE and tested on CurryKitten FPV Simulator (PC).
//

#include "ScanADC.h"
#include "HIDController.h"
#include "global.h"

#define DEBUG_PIN                  2                   // Pull pin 2 to GND to enable USB HID

#define LEFT_STICK_X_ADC            ScanADC::MUX_ADC7   // A0
#define LEFT_STICK_Y_ADC            ScanADC::MUX_ADC6   // A1
#define RIGHT_STICK_X_ADC          ScanADC::MUX_ADC5   // A2
#define RIGHT_STICK_Y_ADC          ScanADC::MUX_ADC4   // A3

static ScanADC &adc_scanner = ScanADC::getInstance();

static HIDController HID_controller;

static bool debug = false;

void setup()
{
    pinMode(DEBUG_PIN, INPUT_PULLUP);
    debug = (digitalRead(DEBUG_PIN) == HIGH);

    if (debug)
    {
        Serial.begin(115200);
        delay(3000);

        const ScanADC::channel_config_t config[] =
        {
            { ScanADC::MUX_1V1, 8 }
        };

        uint16_t sample;
        adc_scanner.begin(config, 1);
        adc_scanner.wait_channel(0);
        sample = adc_scanner.get_sample(0);
        adc_scanner.end();

        Serial.print("Measured ADC reference using internal 1.1V Bandgap reference: ");
        Serial.print(1024 * 1.1f / sample);
        Serial.println("V");
    }
    else
    {
        HID_controller.begin();
    }

    const ScanADC::channel_config_t config[] =
    {
        { LEFT_STICK_X_ADC, 8 },   // YAW
        { LEFT_STICK_Y_ADC, 8 },   // THROTTLE
        { RIGHT_STICK_X_ADC, 8 },  // ROLL
        { RIGHT_STICK_Y_ADC, 8 },  // PITCH
    };

    adc_scanner.begin(config, 4);
}

void loop()
{
    uint8_t sn;
    uint16_t left_x, left_y, right_x, right_y;

    adc_scanner.wait_channel(3);

    sn = adc_scanner.get_sn(3);
    left_x = adc_scanner.get_sample(0);
    left_y = adc_scanner.get_sample(1);
    right_x = adc_scanner.get_sample(2);
    right_y = adc_scanner.get_sample(3);

    if (debug)
    {
        Serial.print(millis(), HEX);
        Serial.print(" sn: ");
        Serial.print(sn, HEX);
        Serial.print(" adc: ");
        Serial.print(left_x, HEX);
        Serial.print(" ");
        Serial.print(left_y, HEX);
        Serial.print(" ");
        Serial.print(right_x, HEX);
        Serial.print(" ");
        Serial.print(right_y, HEX);
        Serial.println();
    }
    else
    {
        HID_controller_report report;

        report.left_stick_x = left_x;
        report.left_stick_y = left_y;
        report.right_stick_x = right_x;
        report.right_stick_y = right_y;
        report.buttons = 0;

        HID_controller.send(report);
    }
}
