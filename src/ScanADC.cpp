/**
 * @file ScanADC.cpp
 * @author Hobbylad ()
 * @brief Library to scan analogue inputs with ADC, measuring and averaging in background under interrupt control.
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
 * Compiled with Arduino 1.8.13 IDE and tested on CurryKitten FPV Simulator (PC). *
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

#include "ScanADC.h"

#include "Arduino.h"
#include <avr/interrupt.h>

ISR(ADC_vect)
{
    ScanADC &adc_scan = ScanADC::getInstance();

    switch (adc_scan.state)
    {
        case ScanADC::ISR_STATE_INIT:
        {
            uint8_t mux = adc_scan.config[adc_scan.chan_i].mux;

            ADCSRB = (ADCSRB & (~(1 << MUX5))) | ((mux & 0x20) ? (1 << MUX5) : 0);
            ADMUX = (ADMUX & 0xE0) | (mux & 0x1F);

            adc_scan.sample_accumulator = 0;
            adc_scan.sample_cnt = 0;
            adc_scan.sample_cnt_target = 1;
            adc_scan.sample_cnt_target <<= adc_scan.config[adc_scan.chan_i].sample_count_log2;

            adc_scan.state = ScanADC::ISR_STATE_DELAY;
        }
        break;

        case ScanADC::ISR_STATE_DELAY:
        {
            adc_scan.state = ScanADC::ISR_STATE_ACCUMULATE;
        }
        break;

        case ScanADC::ISR_STATE_ACCUMULATE:
        {
            uint32_t accumulator = adc_scan.sample_accumulator;
            uint8_t low, high;

            low = ADCL;
            high = ADCH;

            accumulator += (uint16_t)((high << 8) | low);

            if (++adc_scan.sample_cnt == adc_scan.sample_cnt_target)
            {
                uint8_t samples_log2 = adc_scan.config[adc_scan.chan_i].sample_count_log2;

                if (samples_log2 != 0)
                {
                    accumulator += (adc_scan.sample_cnt_target >> 1);
                    accumulator >>= samples_log2;
                }

                adc_scan.sample[adc_scan.chan_i] = (uint16_t) accumulator;
                adc_scan.sn[adc_scan.chan_i]++;

                if (adc_scan.channel_cb)
                {
                    adc_scan.channel_cb(adc_scan.chan_i, (uint16_t) accumulator);
                }

                if (++adc_scan.chan_i == adc_scan.chan_count)
                {
                    if (adc_scan.channel_scan_cb)
                    {
                        adc_scan.channel_scan_cb(adc_scan.sample);
                    }

                    adc_scan.chan_i = 0;
                }

                adc_scan.state = ScanADC::ISR_STATE_INIT;
            }
            else
            {
                adc_scan.sample_accumulator = accumulator;
            }
        }
        break;
    }
}

void ScanADC::begin(const channel_config_t *channel_config, uint8_t channel_count)
{
    end();

    ADCSRB = 0;

    uint16_t config_size = sizeof(channel_config_t) * channel_count,
             sn_size = sizeof(uint8_t) * channel_count,
             sample_size = sizeof(uint16_t) * channel_count,
             alloc_size = config_size + sn_size + sample_size;

    void *p = malloc(alloc_size);
    memset(p, 0, alloc_size);

    config = (channel_config_t *) p;
    p+= config_size;
    sn = (uint8_t *) p;
    p+= sn_size;
    sample = (uint16_t *) p;

    memcpy(config, channel_config, config_size);

    chan_count = channel_count;

    state = ISR_STATE_INIT;
    chan_i = 0;

    ADMUX = (1 << REFS0) | // AVCC reference with external capacitor at AREF pin
            (0 << ADLAR) | // Format of sample ((ADCH << 8) | ADCL)
            config[0].mux; // ADC channel to start

    ADCSRA = (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0) | // Divide clock by 16 for 76.9KHz sample rate
             (1 << ADEN) |                                // ADC enable
             (1 << ADATE) |                               // ADC auto-trigger enable
             (1 << ADIE);                                 // ADC interrupt enable

    ADCSRA |= (1 << ADSC); // ADC start conversion.

    sei(); // Enable global interrupts.
}

void ScanADC::end()
{
    ADCSRA = 0;

    if (config)
    {
        free(config);
        config = NULL;
    }
}

void ScanADC::attach_channel_callback(channel_callback_t cb)
{
    uint8_t old_ADCSRA = ADCSRA;

    ADCSRA &= ~(1 << ADIE);
    channel_cb = cb;
    ADCSRA = old_ADCSRA;
}

void ScanADC::attach_scan_callback(channel_scan_callback_t cb)
{
    uint8_t old_ADCSRA = ADCSRA;

    ADCSRA &= ~(1 << ADIE);
    channel_scan_cb = cb;
    ADCSRA = old_ADCSRA;
}

void ScanADC::wait_channel(uint8_t channel) const
{
    uint8_t last_sn = sn[channel];

    while (last_sn == sn[channel])
    {
        last_sn = sn[channel];
    }
}

void ScanADC::wait_scan() const
{
    if (chan_count > 0)
    {
        wait_channel(chan_count - 1);
    }
}

uint16_t ScanADC::get_sample(uint8_t channel) const
{
    uint16_t s;
    uint8_t old_ADCSRA = ADCSRA;

    ADCSRA &= ~(1 << ADIE);
    s = sample[channel];
    ADCSRA = old_ADCSRA;

    return s;
}
