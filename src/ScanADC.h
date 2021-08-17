/**
 * @file ScanADC.h
 * @author Hobbylad ()
 * @brief Library to scan analogue inputs with ADC measuring and averaging in background under interrupt control.
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

#ifndef ADC_SCAN_H
#define ADC_SCAN_H

#include "stdint.h"
#include "stdlib.h"

#define MAX_CHANNELS 16

/**
 * ADC Interrupt Service Routine (ISR) has C linkage. Declaration used to create
 * a friend of the class to access member variables.
 */
extern "C" void __vector_29();

/**
 * @brief Class to scan analogue inputs with ADC measuring and averaging in background under interrupt control.
 */
class ScanADC
{
    public:

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    /**
     * @brief ATmega328P/ATmega168 Hardware analogue input MUX value.
     *
     * Only available if __AVR_ATmega328P__ or __AVR_ATmega168__ are defined by Arduino environment.
     */
    typedef enum _mux0_t
    {
        MUX_ADC0 = 0,   /**< ADC0 analogue input. */
        MUX_ADC1 = 1,   /**< ADC1 analogue input. */
        MUX_ADC2 = 2,   /**< ADC2 analogue input. */
        MUX_ADC3 = 3,   /**< ADC3 analogue input. */
        MUX_ADC4 = 4,   /**< ADC4 analogue input. */
        MUX_ADC5 = 5,   /**< ADC5 analogue input. */
        MUX_ADC6 = 6,   /**< ADC6 analogue input. */
        MUX_ADC7 = 7,   /**< ADC7 analogue input. */
        MUX_ADC8 = 8,   /**< ADC8 analogue input. */
        MUX_1V1  = 14,  /**< 1.1V internal bandgap. */
        MUX_0V0  = 15   /**< GND. */
    } mux_t;

#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    /**
     * @brief ATmega1280/ATmega2560 Hardware analogue input MUX value.
     *
     * Only available if __AVR_ATmega1280__ or __AVR_ATmega2560__ are defined by Arduino environment.
     */
    typedef enum _mux1_t
    {
        MUX_ADC0 = 0,   /**< ADC0 analogue input. */
        MUX_ADC1 = 1,   /**< ADC1 analogue input. */
        MUX_ADC2 = 2,   /**< ADC2 analogue input. */
        MUX_ADC3 = 3,   /**< ADC3 analogue input. */
        MUX_ADC4 = 4,   /**< ADC4 analogue input. */
        MUX_ADC5 = 5,   /**< ADC5 analogue input. */
        MUX_ADC6 = 6,   /**< ADC6 analogue input. */
        MUX_ADC7 = 7,   /**< ADC7 analogue input. */
        MUX_1V1  = 30,  /**< 1.1V internal bandgap. */
        MUX_0V0  = 31   /**< GND. */
        MUX_ADC8 = 32,  /**< ADC8 analogue input. */
        MUX_ADC9 = 33,  /**< ADC9 analogue input. */
        MUX_ADC10 = 34, /**< ADC10 analogue input. */
        MUX_ADC11 = 35, /**< ADC11 analogue input. */
        MUX_ADC12 = 36, /**< ADC12 analogue input. */
        MUX_ADC13 = 37, /**< ADC13 analogue input. */
        MUX_ADC14 = 38, /**< ADC14 analogue input. */
        MUX_ADC15 = 39, /**< ADC15 analogue input. */
    } mux_t;

#endif

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
    /**
     * @brief ATmega32U4/ATmega16U4 Hardware analogue input MUX value.
     *
     * Only available if __AVR_ATmega32U4__ or __AVR_ATmega16U4__ are defined by Arduino environment.
     */
    typedef enum _mux2_t
    {
        MUX_ADC0 = 0,   /**< ADC0 analogue input. */
        MUX_ADC1 = 1,   /**< ADC1 analogue input. */
        MUX_ADC4 = 4,   /**< ADC4 analogue input. */
        MUX_ADC5 = 5,   /**< ADC5 analogue input. */
        MUX_ADC6 = 6,   /**< ADC6 analogue input. */
        MUX_ADC7 = 7,   /**< ADC7 analogue input. */
        MUX_1V1  = 30,  /**< 1.1V internal bandgap. */
        MUX_0V0  = 31,   /**< GND. */
        MUX_ADC8 = 32,  /**< ADC8 analogue input. */
        MUX_ADC9 = 33,  /**< ADC9 analogue input. */
        MUX_ADC10 = 34, /**< ADC10 analogue input. */
        MUX_ADC11 = 35, /**< ADC11 analogue input. */
        MUX_ADC12 = 36, /**< ADC12 analogue input. */
        MUX_ADC13 = 37, /**< ADC13 analogue input. */
        MUX_TEMP = 39,  /**< Temperature sensor. */
    } mux_t;

#endif

#if !defined(__AVR_ATmega328P__) && !defined(__AVR_ATmega168__) &&  \
    !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__) && \
    !defined(__AVR_ATmega32U4__) && !defined(__AVR_ATmega16U4__)
#error "This library only supports AVR ATmega devices!"
#endif

    /**
    * @brief Structure to hold configuration for a single channel.
    *
    * The structure holds configuration information for a single channel. The #mux field
    * is a hardware specific value that connects an analogue input to the ADC for measurement.
    * The #sample_count_log2 is the log 2 of the sample count to accumulate and average to
    * produce a single sample for the channel. The actual sample count value is 2 to the power
    * of #sample_count_log2.
    */
    struct channel_config_t
    {
        ScanADC::mux_t  mux;           /**< Hardware value to connect analogue input to ADC. */
        uint8_t  sample_count_log2:4;  /**< Log 2 of sample count. */
    };

    /**
    * @brief Definition of the channel measured callback.
    *
    * The channel callback is called after a channel has been measured an will supply the
    * @a channel and the measured @a sample as parameters.
    *
    * Note that the callback is called from the ADC Interrupt Service Routine (ISR) and should
    * be as short as possible. An example use is to push the sample into a queue for processing
    * by the main thread.
    */
    typedef void (*channel_callback_t)(uint8_t channel, uint16_t sample);

    /**
    * @brief Definition of the channel scan measured callback.
    *
    * The channel scan callback is called after all the channels in the configuration have been
    * measured and will supply a pointer to the channel samples #samples.
    *
    * Note that the callback is called from the ADC Interrupt Service Routine (ISR) and should
    * be as short as possible. An example use is to push the samples into a queue for processing
    * by the main thread.
    */
    typedef void (*channel_scan_callback_t)(const uint16_t *samples);

    /**
    * @brief Get the single object of ScanADC.
    *
    * This implement the singleton pattern to ensure only one single object of this class exists.
    * In princible sharing the ADC and Interrupt Service Routine by multiple object could have
    * been engineered but it would add considerable complexity for little gain.
    *
    * @return ScanADC& Instance of ScanADC.
    */
    static inline ScanADC &getInstance()
    {
        static ScanADC adc_scan;
        return adc_scan;
    }

    /**
    * @brief Starts scanning user defined analogue input channnels with the ADC under interrupt control.
    *
    * This function takes in the configuration of the channels to scan as a pointer to the configuration
    * array @a channel_config and channel count @a channel_count and starts the scanning. A copy is made
    * internally of the configuration. The ADC hardware is configured and ADC interrupt enabled
    * for background ADC control, measurement and optional averaging of configured channels in
    * round-robin fashion. The measurement always starts at channel 0 and wraps back after channel
    * @a channel - 1.
    *
    * To stop the scanning call #end().
    *
    * Example from 4-axis RC controller example:
    * @code
    *   #define LEFT_STICK_X_ADC           ScanADC::MUX_ADC7   // A0
    *   #define LEFT_STICK_Y_ADC           ScanADC::MUX_ADC6   // A1
    *   #define RIGHT_STICK_X_ADC          ScanADC::MUX_ADC5   // A2
    *   #define RIGHT_STICK_Y_ADC          ScanADC::MUX_ADC4   // A3
    *
    *   const ScanADC::channel_config_t config[] =
    *   {
    *       { LEFT_STICK_X_ADC, 8 },   // YAW
    *       { LEFT_STICK_Y_ADC, 8 },   // THROTTLE
    *       { RIGHT_STICK_X_ADC, 8 },  // ROLL
    *       { RIGHT_STICK_Y_ADC, 8 },  // PITCH
    *   };
    *
    *   adc_scanner.begin(config, 4);
    * @endcode
    * @param[in] channel_config Pointer to array with channel configurations.
    * @param[in] channel_count  Channel count to configure.
    */
    void begin(const channel_config_t *channel_config, uint8_t channel_count);

    /**
    * @brief Stops scanning disabling interrupt control.
    */
    void end();

    /**
    * @brief Configures callback function to be called after each analogue channel is scanned.
    *
    * The channel callback, if enables, is called after a channel has been measured an will supply
    * the @a channel and the @a sample as parameters.
    *
    * The callback is enabled when a user defined function pointer @a cb is supplied and disabled when
    * NULL is passed as the function pointer.
    *
    * Note that this function is safe to call while scanning is in operation.
    *
    * Note that the callback is called from the ADC Interrupt Service Routine (ISR) and should
    * be as short as possible. An example use is to push the sample into a queue for processing
    * by the main thread.
    *
    * @param[in] cb Pointer to callback function or NULL to disable callback.
    */
    void attach_channel_callback(channel_callback_t cb = NULL);

    /**
    * @brief Configures callback function to be called after all analogue channels are scanned.
    *
    * The channel scan callback, if enables, is called after all the channels in the configuration
    * have been measured and will supply a pointer to the channel samples #samples.
    *
    * The callback is enabled when a user defined function pointer is supplied and disabled when
    * NULL is passed as the function pointer.
    *
    * Note that this function is safe to call while scanning is in operation.
    *
    * Note that the callback is called from the ADC Interrupt Service Routine (ISR) and should
    * be as short as possible. An example use is to push the samples into a queue for processing
    * by the main thread.
    *
    * @param[in] cb Pointer to callback function or NULL to disable callback.
    */
    void attach_scan_callback(channel_scan_callback_t cb = NULL);

    /**
    * @brief Waits until a specified user configured channel has been measured.
    *
    * This function will wait for the sample measurement to finish for @a channel and is useful for
    * synchronising the client with the background measurement. It works by detecting a change in
    * a per channel sequence number that is incremented after a channel measurement.
    *
    * Since channels are scanned and measured in round-bin fashion from 0 to the configured channel
    * count, waiting for a channel is often enough to synchronize to multiple channels in a scan. For
    * instance, waiting for the channel count minus 1 can be used to synchronise to all channels being
    * measured provided the sample from 0 can be read by get_sample() before it is overwritten by the
    * next scan of measurements. The time available for reading channel 0 is dependent on the ADC clock
    * rate and the number of samples being averaged. Since a sample is measured every 12us, this time
    * is approximately 12us times (2 to the power of @a sample_count_log2).
    *
    * If this function is used to wait for continuous samples, it must always be running before
    * @a channel measurement completes otherwise samples will be skipped.
    *
    * Example from 4-axis RC controller example:
    * @code
    *   adc_scanner.wait_channel(3);
    *
    *   sn = adc_scanner.get_sn(3);
    *   left_x = adc_scanner.get_sample(0);
    *   left_y = adc_scanner.get_sample(1);
    *   right_x = adc_scanner.get_sample(2);
    *   right_y = adc_scanner.get_sample(3);
    * @endcode
    * @param[in] channel Channel index.
    */
    void wait_channel(uint8_t channel) const;

    /**
    * @brief Waits until all the user configured channels have been measured.
    *
    * This is equvalent to wait_channel(@a channel_count - 1).
    */
    void wait_scan() const;

    /**
    * @brief Get the sample sequence number for a channel.
    *
    * Each channel has an individual cycling sequence number counter from 0 to 255 that is incremented
    * once a measurement is completed and available for the channel. For a scan all the indexes will be
    * the same for all channels once all the channels in the scan have beem measured. The sequence numbers
    * for all channels are zeroed by begin().
    *
    * Note this function is always safe to call even without client synchronisation via wait_channel().
    *
    * @param  channel Channel index.
    * @return uint8_t Sequence number cycling from zero to 255.
    */
    inline uint8_t get_sn(uint8_t channel) const
    {
        return sn[channel];
    }

    /**
    * @brief Reads sample for a user configure channel.
    *
    * This returns the last measured 10-bit sample for a channel.
    *
    * Note this function is always safe to call even without client synchronisation via wait_channel().
    *
    * @param[in] channel Channel index.
    * @return uint16_t 10-bit unsigned sample.
    */
    uint16_t get_sample(uint8_t channel) const;

    private:

    /**
     * @brief Private constructor to ensure only getInstance() can create this object.
     */
    ScanADC() : channel_cb(NULL), channel_scan_cb(NULL)
    {
    }

    /**
    * @brief Prevent copy constructor.
    */
    ScanADC(const ScanADC &) = delete;

    /**
    * @brief Prevent assignment.
    */
    ScanADC& operator=(const ScanADC &) = delete;

    /**
    * @brief ADC Interrupt Service Routine (ISR) declared as friend to allow access to member variables.
    */
    friend void __vector_29();

    /**
     * @brief Structure to hold channel results.
     */
    struct channel_result_t
    {
        volatile uint16_t sample;             /**< Channel 12-bit sample. */
        volatile uint8_t sn;                  /**< Channel sequence number (0 to 255). */
    };

    /**
    * @brief Interrupt Service Routine (ISR) state machine states.
    */
    enum isr_state_t
    {
      ISR_STATE_INIT = 0,                      /**< Initialises channel measurement. */
      ISR_STATE_DELAY,                         /**< Delay required after switching analogue input. */
      ISR_STATE_ACCUMULATE                     /**< Accumulates and when done, advances to next channel. */
    };

    uint8_t chan_count;                        // Channel count configured.

    channel_callback_t channel_cb;             // Callback after channel processed.
    channel_scan_callback_t channel_scan_cb;   // Callback after all channels processed.

    isr_state_t state;                         // Sequencing state.
    uint8_t chan_i;                            // Channel index being processed.

    uint16_t sample_cnt;                       // Sample counter (0 to sample_cnt_target).
    uint16_t sample_cnt_target;                // Sample count to accumulate.
    uint32_t sample_accumulator;               // Sample accumulator.

    channel_config_t *config;                  // Channel configurations.
    volatile uint8_t *sn;                      // Channel sample sequence numbers.
    volatile uint16_t *sample;                 // Channel sample values.
};


#endif
