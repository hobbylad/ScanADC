# ScanADC

## Introduction

ScanADC is an Arduino library for scanning configurable list of analogue inputs with the ATmega ADC, measuring and averaging samples in background under interrupt control. It leaves the main loop thread available for other processing. The main thread still needs to poll to retrieving samples at certain points but a callback feature is available to allow user queueing of samples. The library has been kept as simple as possible.

The ADC clock prescaler is fixed at 16 and generates a samples at a rate of 76.9KHz with 10-bit resolution.

## Usage

**Get singleton instance of scanner:**

    ScanADC &adc_scanner = ScanADC::getInstance();

**Configure the required channel count (4), analogue inputs and averaging (1 << 8 = 256 samples) to scan:**

    #define LEFT_STICK_X_ADC           ScanADC::MUX_ADC7   // A0
    #define LEFT_STICK_Y_ADC           ScanADC::MUX_ADC6   // A1
    #define RIGHT_STICK_X_ADC          ScanADC::MUX_ADC5   // A2
    #define RIGHT_STICK_Y_ADC          ScanADC::MUX_ADC4   // A3

    const ScanADC::channel_config_t config[] =
    {
        { LEFT_STICK_X_ADC, 8 },   // YAW
        { LEFT_STICK_Y_ADC, 8 },   // THROTTLE
        { RIGHT_STICK_X_ADC, 8 },  // ROLL
        { RIGHT_STICK_Y_ADC, 8 },  // PITCH
    };

**Start scanning the ADC channels:**

    adc_scanner.begin(config, 4);

**Wait for last ADC channel in list to be updated in service loop such as main() and read the channel samples:**

    adc_scanner.wait_channel(3);

    sn = adc_scanner.get_sn(3);
    left_x = adc_scanner.get_sample(0);
    left_y = adc_scanner.get_sample(1);
    right_x = adc_scanner.get_sample(2);
    right_y = adc_scanner.get_sample(3);

In this example, each channel sample is produced by 256 averaged ADC samples. There is time available after channel 3 is updated and the wait unblocks to read channel 0 (and 1  to 3) before they are updated in the new scan.

## Example

**Simple USB drone controller for PC simulator: [USBDroneController.ino](examples/USBDroneController/USBDroneController.ino).**

Prototyped on 5V Pro Micro ATMega32U4 (Arduino Leonardo) and wired to hacked left & right 2-axis joystick drone controller outputing 0 - 3.3V per axis. Appears at HID standard device and does not need custom driver. Use axis calibration and axis "invert" if necessary in drone simulator controller setup. Compiled with Arduino 1.8.13 IDE and tested on CurryKitten FPV Simulator (PC).

## Documentation

The documentation is generated with Doxygen with the Doxyfile configuration file. 

See [generated Doxygen documentation](http://hobbylad.github.io/ScanADC/index.html)

## Supported architectures:

+ __AVR_ATmega328P__, __AVR_ATmega168__
+ __AVR_ATmega1280__, __AVR_ATmega2560__
+ __AVR_ATmega32U4__, __AVR_ATmega16U4__

## Release Notes

v0.1    15 July 2021:
- Initial release
