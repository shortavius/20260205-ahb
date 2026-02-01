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

#if !defined(LEDBAR_H)
#define LEDBAR_H

//
// Shared Includes
//
#include <stdint.h>

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
// Shared Definitions
//

//
// Shared Structures / Enumerations / Type Definitions
//
enum led_commands
{
    LED_COMMAND_ON,
    LED_COMMAND_OFF,
    LED_COMMAND_BLINK,
    LED_COMMAND_MAX
};

void ledbar_cfg(void);
void ledbar_set_led_mode(
    int led,
    enum led_commands cmd);
void ledbar_set_blink_rate_msecs(
    int led,
    uint64_t delay);
void ledbar_action();

END_C_DECLS

#endif // LEDBAR_H
