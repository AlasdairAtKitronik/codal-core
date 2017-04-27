/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef DEVICE_BUTTON_H
#define DEVICE_BUTTON_H

#include "AbstractButton.h"
#include "DeviceConfig.h"
#include "DeviceComponent.h"
#include "DeviceEvent.h"
#include "Pin.h"

namespace codal
{
    /**
      * Class definition for Device Button.
      *
      * Represents a single, generic button on the device.
      */
    class DeviceButton : public AbstractButton
    {
        unsigned long downStartTime;                            // used to store the current system clock when a button down event occurs
        uint8_t sigma;                                          // integration of samples over time. We use this for debouncing, and noise tolerance for touch sensing
        DeviceButtonEventConfiguration eventConfiguration;      // Do we want to generate high level event (clicks), or defer this to another service.
        DeviceButtonPolarity polarity;                          // Determines if the button is active HIGH or LOW.

        public:
        Pin &_pin;                                        // The pin this button is connected to.

        /**
          * Constructor.
          *
          * Create a software representation of a button.
          *
          * @param pin the physical pin on the device connected to this button.
          *
          * @param id the ID of the new DeviceButton object.
          *
          * @param eventConfiguration Configures the events that will be generated by this DeviceButton instance.
          *                           Defaults to DEVICE_BUTTON_ALL_EVENTS.
          *
          * @param mode the configuration of internal pullups/pulldowns, as defined in the mbed PinMode class. PullNone by default.
          *
          * @code
          * buttonA(DEVICE_PIN_BUTTON_A, DEVICE_ID_BUTTON_A);
          * @endcode
          */
        DeviceButton(Pin &pin, uint16_t id, DeviceButtonEventConfiguration eventConfiguration = DEVICE_BUTTON_ALL_EVENTS, DeviceButtonPolarity polarity = ACTIVE_LOW, PinMode mode = PullNone);

        /**
          * Tests if this Button is currently pressed.
          *
          * @code
          * if(buttonA.isPressed())
          *     display.scroll("Pressed!");
          * @endcode
          *
          * @return 1 if this button is pressed, 0 otherwise.
          */
        virtual int isPressed();

        /**
          * Changes the event configuration used by this button to the given DeviceButtonEventConfiguration.
          *
          * All subsequent events generated by this button will then be informed by this configuraiton.
          *
          * @param config The new configuration for this button. Legal values are DEVICE_BUTTON_ALL_EVENTS or DEVICE_BUTTON_SIMPLE_EVENTS.
          *
          * Example:
          * @code
          * // Configure a button to generate all possible events.
          * buttonA.setEventConfiguration(DEVICE_BUTTON_ALL_EVENTS);
          *
          * // Configure a button to suppress DEVICE_BUTTON_EVT_CLICK and DEVICE_BUTTON_EVT_LONG_CLICK events.
          * buttonA.setEventConfiguration(DEVICE_BUTTON_SIMPLE_EVENTS);
          * @endcode
          */
        void setEventConfiguration(DeviceButtonEventConfiguration config);

        /**
         * periodic callback from Device system timer.
         *
         * Check for state change for this button, and fires various events on a state change.
         */
        void periodicCallback();

        /**
          * Destructor for DeviceButton, where we deregister this instance from the array of fiber components.
          */
        ~DeviceButton();

        protected:
        /**
         * Determines if this button is instantenously active (i.e. pressed).
         * Internal method, use before debouncing.
         */
        virtual int buttonActive();

    };
}

#endif
