#include "hrtbt.h"

#include <Arduino.h>

#define HRTBT_LED_PIN                       LED_BUILTIN
#define HRTBT_DELAY_MSECS                   1000

// This enumeration is used to define tehe states the LED could be in.
typedef enum {
    // Indicates that the heartbeat LED is on
    HRTBT_LED_STATE_ON,

    // Indicates that the heartbeat LED is off
    HRTBT_LED_STATE_OFF,

    // Indicates the maximum possbile states of the LED
    HRTBT_LED_STATE_MAX
} hrtbt_led_state_t;

// This local global variable is used to determine when something needs to be
// done to the LED.
static unsigned long hrtbt_toggle_time_msecs = 0;

// This local global variable is used to keep track of the current state of the LED
static hrtbt_led_state_t hrtbt_led_state = HRTBT_LED_STATE_MAX;

// This local global variable is used to hold the delay interval for the LED.
static unsigned long hrtbt_delay_ms = (unsigned long)HRTBT_DELAY_MSECS;


void hrtbt_cfg(void)
{
    // Set the LED_BUILTIN as an output so we may control it. The way this pin is
    // connected to the LED, if we put a HIGH on the pin, it will turn on the LED
    // while a LOW will turn off the LED.
    pinMode((pin_size_t)HRTBT_LED_PIN, OUTPUT);

    // Turn off the LED
    digitalWrite(LED_BUILTIN, LOW);

    // Update the current state of the LED
    hrtbt_led_state = HRTBT_LED_STATE_OFF;
}

void hrtbt_action(void)
{
    const unsigned long msecs = millis();

    if (hrtbt_toggle_time_msecs <= msecs)
    {
        if (HRTBT_LED_STATE_OFF == hrtbt_led_state)
        {
            // Turn on the LED
            digitalWrite((pin_size_t)HRTBT_LED_PIN, HIGH);

            // Update the LED state
            hrtbt_led_state = HRTBT_LED_STATE_ON;
        }
        else
        {
            // Turn off the LED
            digitalWrite((pin_size_t)HRTBT_LED_PIN, LOW);

            // Update the LED state
            hrtbt_led_state = HRTBT_LED_STATE_OFF;
        }

        // Update the next time the LED needs to have something done to it
        hrtbt_toggle_time_msecs = msecs + hrtbt_delay_ms;
    }
}

unsigned long hrtbt_get_delay(void)
{
    return hrtbt_delay_ms;
}

void hrtbt_set_delay(unsigned long new_delay)
{
    hrtbt_delay_ms = new_delay;
}
