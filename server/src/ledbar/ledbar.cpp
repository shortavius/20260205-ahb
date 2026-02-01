///
/// @file
///
/// @brief
/// Led bar GPIO interface
///
/// @details
/// This module deals with the GPIO contorl for a 5-segment LED bar. It allows
/// for setting the individual LEDs to either be on, off, or blink. When the
/// LED is set to blink, the delay between blinking may be set.

//
// Local Includes
//
#include "ledbar.h"

#include <Arduino.h>


#undef BEGIN_C_DECLS
#undef END_C_DECLS
#if defined(__cplusplus)
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS }
#else
#  define BEGIN_C_DECLS /* empty */
#  define END_C_DECLS /* empty */
#endif

BEGIN_C_DECLS

//
// Local Definitions
//
#define BAR_LED_COUNT                       5

#define LED_BAR_1_PIN                       12
#define LED_BAR_2_PIN                       11
#define LED_BAR_3_PIN                       10
#define LED_BAR_4_PIN                       9
#define LED_BAR_5_PIN                       8

#define LED_DEFAULT_BLINK_MSECS             500

//
// Local Structures / Enumerations / Type Definitions
//
enum pin_active_types
{
    PIN_ACTIVE_LOW,
    PIN_ACTIVE_HIGH,
    PIN_ACTIVE_MAX
};

enum pin_states
{
    PIN_STATE_ON,
    PIN_STATE_OFF,
    PIN_STATE_MAX
};

struct led_pin
{
    enum pin_active_types active_type;
    pin_size_t number;
    enum pin_states state;
};

enum led_modes
{
    LED_MODE_STATIC,
    LED_MODE_BLINK,
    LED_MODE_MAX
};

typedef struct
{
    enum led_modes mode;
    led_pin pin;
    uint64_t delay_msecs;
    uint64_t next_toggle_time_msecs;
} led_t;


static led_t ledbar_leds[BAR_LED_COUNT] =
{
    {
        .mode = (enum led_modes)LED_MODE_STATIC,
        .pin =
        {
            .active_type = (enum pin_active_types)PIN_ACTIVE_LOW,
            .number = (pin_size_t)LED_BAR_1_PIN,
            .state = (enum pin_states)PIN_STATE_OFF
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
    },
    {
        .mode = (enum led_modes)LED_MODE_STATIC,
        .pin =
        {
            .active_type = (enum pin_active_types)PIN_ACTIVE_LOW,
            .number = (pin_size_t)LED_BAR_2_PIN,
            .state = (enum pin_states)PIN_STATE_OFF
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
   },
    {
        .mode = (enum led_modes)LED_MODE_STATIC,
        .pin =
        {
            .active_type = (enum pin_active_types)PIN_ACTIVE_LOW,
            .number = (pin_size_t)LED_BAR_3_PIN,
            .state = (enum pin_states)PIN_STATE_OFF
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
    },
    {
        .mode = (enum led_modes)LED_MODE_STATIC,
        .pin =
        {
            .active_type = (enum pin_active_types)PIN_ACTIVE_LOW,
            .number = (pin_size_t)LED_BAR_4_PIN,
            .state = (enum pin_states)PIN_STATE_OFF
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
    },
    {
        .mode = (enum led_modes)LED_MODE_STATIC,
        .pin =
        {
            .active_type = (enum pin_active_types)PIN_ACTIVE_LOW,
            .number = (pin_size_t)LED_BAR_5_PIN,
            .state = (enum pin_states)PIN_STATE_OFF
        },
        .delay_msecs = (uint64_t)LED_DEFAULT_BLINK_MSECS,
        .next_toggle_time_msecs = (uint64_t)0
    }
};

static char * mode_msg_blinking = "BLINKING";
static char * mode_msg_static_on = "STATIC ON";
static char * mode_msg_static_off = "STATIC OFF";

static void pin_assert(led_pin * p_pin);
static void pin_deassert(led_pin * p_pin);

void ledbar_cfg(void)
{
    for(
       int idx = 0;
       (int)BAR_LED_COUNT > idx;
       ++idx)
    {
        pinMode(ledbar_leds[idx].pin.number, OUTPUT);
        pin_deassert(&ledbar_leds[idx].pin);
    }
}

uint64_t ledbar_get_delay_msecs(int led)
{
    uint64_t rv = 0u;

    if ((int)BAR_LED_COUNT > led)
    {
        rv = ledbar_leds[led].delay_msecs;
    }

    return rv;
}

char * ledbar_get_mode(int led)
{
    char * rv = NULL;

    if ((int)BAR_LED_COUNT > led)
    {
        if ((enum led_modes)LED_MODE_BLINK == ledbar_leds[led].mode)
        {
            rv = mode_msg_blinking;
        }
        else
        {
            if ((enum pin_states)PIN_STATE_ON == ledbar_leds[led].pin.state)
            {
                rv = mode_msg_static_on;
            }
            else
            {
                rv = mode_msg_static_off;
            }
        }
    }

    return rv;
}

void ledbar_set_led_mode(
    int led,
    enum led_commands cmd)
{
    if ((int)BAR_LED_COUNT <= led) { return; }

    if (LED_COMMAND_BLINK == cmd)
    {
        ledbar_leds[led].mode = LED_MODE_BLINK;
    }
    else
    {
        if (ledbar_leds[led].mode != LED_MODE_STATIC)
        {
            ledbar_leds[led].mode = LED_MODE_STATIC;
        }

        if (LED_COMMAND_ON == cmd)
        {
            pin_assert(&ledbar_leds[led].pin);
        }
        else
        {
            pin_deassert(&ledbar_leds[led].pin);
        }
    }
}

void ledbar_set_blink_rate_msecs(
    int led,
    uint64_t delay)
{
    if ((int)BAR_LED_COUNT <= led) { return; }

    ledbar_leds[led].delay_msecs = delay;
}

void ledbar_action()
{
    const uint64_t curr_time = millis();

    for (
        int idx = 0u;
        (int)BAR_LED_COUNT > idx;
        ++idx)
    {
        if ((LED_MODE_STATIC == ledbar_leds[idx].mode) ||
           (ledbar_leds[idx].next_toggle_time_msecs > curr_time)) { continue; }

        if (PIN_STATE_ON == ledbar_leds[idx].pin.state)
        {
            pin_deassert(&ledbar_leds[idx].pin);
        }
        else
        {
            pin_assert(&ledbar_leds[idx].pin);
        }

        ledbar_leds[idx].next_toggle_time_msecs +=
                                                ledbar_leds[idx].delay_msecs;
    }
}

static void pin_assert(led_pin * p_pin)
{
    if (PIN_ACTIVE_LOW == p_pin->active_type)
    {
        digitalWrite(p_pin->number, LOW);
    }
    else
    {
        digitalWrite(p_pin->number, HIGH);
    }
    p_pin->state = PIN_STATE_ON;
}

static void pin_deassert(led_pin * p_pin)
{
    if (PIN_ACTIVE_LOW == p_pin->active_type)
    {
        digitalWrite(p_pin->number, HIGH);
    }
    else
    {
        digitalWrite(p_pin->number, LOW);
    }
    p_pin->state = PIN_STATE_OFF;
}

END_C_DECLS
