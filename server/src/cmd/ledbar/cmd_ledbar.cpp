///
/// @file
///
/// @brief
/// Led bar command processing (source)
///
/// @details
/// This module handles the CLI commands that interface with the LED bar. This
/// allows for setting the state of each segment in the LED bar along with
/// whether that segment is blinking or state along with the delay during the
/// blinking state.

//
// Local Includes
//
#include "cmd_ledbar.h"
#include "../../cpe/cpe.h"
#include "../../ledbar/ledbar.h"

#include <stdlib.h>
#include <inttypes.h>

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
#define LEDBAR_NUM_COMMANDS                 14

//
// Local Structures / Enumerations / Type Definitions
//

//
// Local Function Prototypes
//
static void cmd_ledbar_help(void *x);
static void cmd_ledbar_get_help(void *x);
static void cmd_ledbar_set_help(void *x);
static void cmd_ledbar_get_int_delay(void *x);
static void cmd_ledbar_get_int_help(void *x);
static void cmd_ledbar_get_int_mode(void *x);
static void cmd_ledbar_set_int_help(void *x);
static void cmd_ledbar_set_int_delay_help(void *x);
static void cmd_ledbar_set_int_delay_int(void *x);
static void cmd_ledbar_set_int_mode_help_tkns(void *x);
static void cmd_ledbar_set_int_mode_blink_tkns (void *x);
static void cmd_ledbar_set_int_mode_static_help_tkns (void *x);
static void cmd_ledbar_set_int_mode_static_on_tkns (void *x);
static void cmd_ledbar_set_int_mode_static_off_tkns (void *x);

//
// Local Global Variables
//
const static struct cpe_syntax_tkn syntax_ledbar_help_tkns[2] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_get_help_tkns[3] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_GET
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_help_tkns[3] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_get_int_help_tkns[4] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_GET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_get_int_mode_tkns[4] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_GET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_MODE
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_get_int_delay_tkns[4] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_GET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_DELAY
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_int_help_tkns[4] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_int_delay_help_tkns[5] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_DELAY
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_int_delay_int_tkns[5] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_DELAY
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_int_mode_help_tkns[5] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_MODE
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_int_mode_blink_tkns[5] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_MODE
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_BLINK
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_int_mode_static_help_tkns[6] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_MODE
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_STATIC
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_int_mode_static_on_tkns[6] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_MODE
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_STATIC
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_ON
    }
};

const static struct cpe_syntax_tkn syntax_ledbar_set_int_mode_static_off_tkns[6] =
{
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_LEDBAR
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
    },
    {
        .cat = CPE_TOKEN_CAT_INTEGER,
        .kyw = CPE_KEYWORD_UNDEFINED
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_MODE
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_STATIC
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_OFF
    }
};

const struct cpe_cmd_syntax syntax_cmd_ledbar[LEDBAR_NUM_COMMANDS] =
{
    {
        .count = 2,
        .syntax_tkns = &syntax_ledbar_help_tkns[0],
        .action = cmd_ledbar_help
    },
    {
        .count = 3,
        .syntax_tkns = &syntax_ledbar_get_help_tkns[0],
        .action = cmd_ledbar_get_help
    },
    {
        .count = 3,
        .syntax_tkns = &syntax_ledbar_set_help_tkns[0],
        .action = cmd_ledbar_set_help
    },
    {
        .count = 4,
        .syntax_tkns = &syntax_ledbar_get_int_delay_tkns[0],
        .action = cmd_ledbar_get_int_delay
    },
    {
        .count = 4,
        .syntax_tkns = &syntax_ledbar_get_int_help_tkns[0],
        .action = cmd_ledbar_get_int_help
    },
    {
        .count = 4,
        .syntax_tkns = &syntax_ledbar_get_int_mode_tkns[0],
        .action = cmd_ledbar_get_int_mode
    },
    {
        .count = 4,
        .syntax_tkns = &syntax_ledbar_set_int_help_tkns[0],
        .action = cmd_ledbar_set_int_help
    },
    {
        .count = 5,
        .syntax_tkns = &syntax_ledbar_set_int_delay_help_tkns[0],
        .action = cmd_ledbar_set_int_delay_help
    },
    {
        .count = 5,
        .syntax_tkns = &syntax_ledbar_set_int_delay_int_tkns[0],
        .action = cmd_ledbar_set_int_delay_int
    },
    {
        .count = 5,
        .syntax_tkns = &syntax_ledbar_set_int_mode_blink_tkns[0],
        .action = cmd_ledbar_set_int_mode_blink_tkns
    },
    {
        .count = 5,
        .syntax_tkns = &syntax_ledbar_set_int_mode_help_tkns[0],
        .action = cmd_ledbar_set_int_mode_help_tkns
    },
    {
        .count = 6,
        .syntax_tkns = &syntax_ledbar_set_int_mode_static_help_tkns[0],
        .action = cmd_ledbar_set_int_mode_static_help_tkns
    },
    {
        .count = 6,
        .syntax_tkns = &syntax_ledbar_set_int_mode_static_on_tkns[0],
        .action = cmd_ledbar_set_int_mode_static_on_tkns
    },
    {
        .count = 6,
        .syntax_tkns = &syntax_ledbar_set_int_mode_static_off_tkns[0],
        .action = cmd_ledbar_set_int_mode_static_off_tkns
    },
};

//
// Code
//
void cmd_ledbar_root_help(void)
{
    struct uart_funcs * uart = cpe_get_info()->uart;

    console_printf(uart, "      ledbar - Issue commands to the ledbar module");
    console_printf(uart, "\r\n");
}

uint8_t cmd_ledbar_syntax(void)
{
    int idx = (int)0;
    int idy = (int)0;
    uint8_t rv = (uint8_t)0;
    uint8_t match = (uint8_t)1;
    struct cpe_info * info = cpe_get_info();
    void (*action)(void * x) = NULL;

    while ((idx < (int)LEDBAR_NUM_COMMANDS) && !rv)
    {
        if (syntax_cmd_ledbar[idx].count != info->token_cnt)
        {
            idx++;
            continue;
        }

        idy = (int)0;
        match = (uint8_t)1;
        while ((idy < syntax_cmd_ledbar[idx].count) && match)
        {
            struct cpe_syntax_tkn tmp = {
                .cat = info->token[idy].cat,
                .kyw = info->token[idy].kyw
            };

            if (!(cpe_cmp_syntax_tkns(syntax_cmd_ledbar[idx].syntax_tkns[idy], tmp)))
            {
                match = 0;
            }

            idy++;
        }

        if (match && (syntax_cmd_ledbar[idx].count == idy))
        {
            rv = 1;
            action = syntax_cmd_ledbar[idx].action;
        }

        idx++;
    }

    if (NULL != action)
    {
        action((void *)NULL);
    }

    return rv;
}

static void cmd_ledbar_help(void *x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "ledbar command actions\r\n\r\n");
    console_printf(uart,
        "get - Retrieve the current value for a setting\r\n");
    console_printf(uart,
        "set - Institute a given value for a setting\r\n");
}

static void cmd_ledbar_get_help(void *x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "ledbar get command LED choice\r\n\r\n");
    console_printf(uart,
        "led# - Which LED to act on [0,1,2,3,4]\r\n");
}

static void cmd_ledbar_set_help(void *x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "ledbar set command LED choice\r\n\r\n");
    console_printf(uart,
        "led# - Which LED to act on [0,1,2,3,4]\r\n");
}

static void cmd_ledbar_get_int_help(void *x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "ledbar get command setting choice\r\n\r\n");
    console_printf(uart,
        "delay - The delay between LED state changes\r\n");
    console_printf(uart,
        " mode - The mode the LED is operating in\r\n");
}

static void cmd_ledbar_set_int_help(void *x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "ledbar set command LED choice\r\n\r\n");
    console_printf(uart,
        "delay - The delay between LED state changes\r\n");
    console_printf(uart,
        " mode - The mode the LED is operating in\r\n");
}

static void cmd_ledbar_get_int_delay(void *x)
{
    (void)x;
    char * end;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = info->uart;
    const long val = strtol(info->token[2].start, &end, 10);

    if ((0 > val) || (4 < val))
    {
        console_printf(uart,
            "The LED number needs to be between 0 and 4.\r\n");
        return;
    }
    unsigned resp = (unsigned)ledbar_get_delay_msecs((int)val);
    console_printf(uart,
        "LED %d delay setting is %u milliseconds\r\n", (int)val, resp);
}

static void cmd_ledbar_get_int_mode(void *x)
{
    (void)x;
    char * end;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = info->uart;
    const long val = strtol(info->token[2].start, &end, 10);

    if ((0 > val) || (4 < val))
    {
        console_printf(uart,
            "The LED number needs to be between 0 and 4.\r\n");
        return;
    }
    console_printf(uart,
        "LED%d mode setting is %s\r\n", (int)val, ledbar_get_mode((int)val));
}

static void cmd_ledbar_set_int_delay_help(void *x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "ledbar set command LED delay setting\r\n\r\n");
    console_printf(uart,
        "msec# - Integer delay between LED state changes in milliseconds\r\n");
}

static void cmd_ledbar_set_int_delay_int(void *x)
{
    (void)x;
    char * end;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = info->uart;
    const long led = strtol(info->token[2].start, &end, 10);
    const long delay = strtol(info->token[4].start, &end, 10);

    if ((0 > led) || (4 < led))
    {
        console_printf(uart,
            "The LED number needs to be between 0 and 4.\r\n");
        return;
    }

    ledbar_set_blink_rate_msecs((int)led, (uint64_t)delay);
    unsigned resp = (unsigned)ledbar_get_delay_msecs((int)led);
    console_printf(uart,
        "LED %d delay setting is %u milliseconds\r\n", (int)led, resp);
}

static void cmd_ledbar_set_int_mode_blink_tkns(void *x)
{
    (void)x;
    char * end;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = cpe_get_info()->uart;
    const long led = strtol(info->token[2].start, &end, 10);

    if ((0 > led) || (4 < led))
    {
        console_printf(uart,
            "The LED number needs to be between 0 and 4.\r\n");
        return;
    }

    ledbar_set_led_mode((int)led, (enum led_commands)LED_COMMAND_BLINK);
    console_printf(uart,
        "LED%d mode setting is %s\r\n", (int)led, ledbar_get_mode((int)led));
}

static void cmd_ledbar_set_int_mode_help_tkns (void *x)
{
    (void)x;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = cpe_get_info()->uart;

    console_printf(uart,
        "ledbar set command LED mode setting\r\n\r\n");
    console_printf(uart,
        " blink - Make the LED blink\r\n");
    console_printf(uart,
        "static - Make the LED hold a specified state\r\n");
}

static void cmd_ledbar_set_int_mode_static_help_tkns (void *x)
{
    (void)x;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = cpe_get_info()->uart;

    console_printf(uart,
        "ledbar set command LED static mode setting\r\n\r\n");
    console_printf(uart,
        "off - Make the LED be off constantly\r\n");
    console_printf(uart,
        " on - Make the LED be on constantly\r\n");
}

static void cmd_ledbar_set_int_mode_static_on_tkns (void *x)
{
    (void)x;
    char * end;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = cpe_get_info()->uart;
    const long led = strtol(info->token[2].start, &end, 10);

    if ((0 > led) || (4 < led))
    {
        console_printf(uart,
            "The LED number needs to be between 0 and 4.\r\n");
        return;
    }

    ledbar_set_led_mode((int)led, (enum led_commands)LED_COMMAND_ON);
    console_printf(uart,
        "LED%d mode setting is %s\r\n", (int)led, ledbar_get_mode((int)led));
}

static void cmd_ledbar_set_int_mode_static_off_tkns (void *x)
{
    (void)x;
    char * end;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = cpe_get_info()->uart;
    const long led = strtol(info->token[2].start, &end, 10);

    if ((0 > led) || (4 < led))
    {
        console_printf(uart,
            "The LED number needs to be between 0 and 4.\r\n");
        return;
    }

    ledbar_set_led_mode((int)led, (enum led_commands)LED_COMMAND_OFF);
    console_printf(uart,
        "LED%d mode setting is %s\r\n", (int)led, ledbar_get_mode((int)led));
}

END_C_DECLS
