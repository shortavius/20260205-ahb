///
/// @file
///
/// @brief
///
///
/// @details
///

//
// Local Includes
//
#include "cmd_hrtbt.h"
#include "../../cpe/cpe.h"
#include "../../hrtbt/hrtbt.h"

#include <stdio.h>
#include <stdlib.h>

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
#define HRTBT_NUM_COMMANDS                 6

//
// Local Structures / Enumerations / Type Definitions
//

//
// Local Function Prototypes
//
static void cmd_hrtbt_help(void * x);
static void cmd_hrtbt_get_set_help(void * x);
static void cmd_hrtbt_get_delay(void * x);
static void cmd_hrtbt_set_delay_help(void * x);
static void cmd_hrtbt_set_delay(void * x);

//
// Local Global Variables
//
const static struct cpe_syntax_tkn syntax_hrtbt_help_tkns[2] = {
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HRTBT
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HELP
    }
};

const static struct cpe_syntax_tkn syntax_hrtbt_get_help_tkns[3] = {
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HRTBT
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

const static struct cpe_syntax_tkn syntax_hrtbt_set_help_tkns[3] = {
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HRTBT
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

const static struct cpe_syntax_tkn syntax_hrtbt_get_delay_tkns[3] = {
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HRTBT
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_GET
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_DELAY
    }
};

const static struct cpe_syntax_tkn syntax_hrtbt_set_delay_help_tkns[4] = {
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HRTBT
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
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

const static struct cpe_syntax_tkn syntax_hrtbt_set_delay_int_tkns[4] = {
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_HRTBT
    },
    {
        .cat = CPE_TOKEN_CAT_KEYWORD,
        .kyw = CPE_KEYWORD_SET
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

const struct cpe_cmd_syntax syntax_cmd_hrtbt[HRTBT_NUM_COMMANDS] = {
    {
        .count = 2,
        .syntax_tkns = &syntax_hrtbt_help_tkns[0],
        .action = cmd_hrtbt_help
    },
    {
        .count = 3,
        .syntax_tkns = &syntax_hrtbt_get_help_tkns[0],
        .action = cmd_hrtbt_get_set_help
    },
    {
        .count = 3,
        .syntax_tkns = &syntax_hrtbt_set_help_tkns[0],
        .action = cmd_hrtbt_get_set_help
    },
    {
        .count = 3,
        .syntax_tkns = &syntax_hrtbt_get_delay_tkns[0],
        .action = cmd_hrtbt_get_delay
    },
    {
        .count = 4,
        .syntax_tkns = &syntax_hrtbt_set_delay_help_tkns[0],
        .action = cmd_hrtbt_set_delay_help
    },
    {
        .count = 4,
        .syntax_tkns = &syntax_hrtbt_set_delay_int_tkns[0],
        .action = cmd_hrtbt_set_delay
    }
};

//
// Code
//
void cmd_hrtbt_root_help(void)
{
    struct uart_funcs * uart = cpe_get_info()->uart;

    console_printf(uart, "      hrtbt - Issue commands to the heartbeat ");
    console_printf(uart, "module\r\n");
}

uint8_t cmd_hrtbt_syntax(void)
{
    int idx = (int)0;
    int idy = (int)0;
    uint8_t rv = (uint8_t)0;
    uint8_t match = (uint8_t)1;
    struct cpe_info * info = cpe_get_info();
    void (*action)(void * x) = NULL;

    while ((idx < (int)HRTBT_NUM_COMMANDS) && !rv)
    {
        if (syntax_cmd_hrtbt[idx].count != info->token_cnt)
        {
            idx++;
            continue;
        }

        idy = (int)0;
        match = (uint8_t)1;
        while ((idy < syntax_cmd_hrtbt[idx].count) && match)
        {
            struct cpe_syntax_tkn tmp = {
                .cat = info->token[idy].cat,
                .kyw = info->token[idy].kyw
            };

            if (!(cpe_cmp_syntax_tkns(syntax_cmd_hrtbt[idx].syntax_tkns[idy], tmp)))
            {
                match = 0;
            }

            idy++;
        }

        if (match && (syntax_cmd_hrtbt[idx].count == idy))
        {
            rv = 1;
            action = syntax_cmd_hrtbt[idx].action;
        }

        idx++;
    }

    if (NULL != action)
    {
        action((void *)NULL);
    }

    return rv;
}

static void cmd_hrtbt_get_delay(void * x)
{
    (void)x;
    const int val = hrtbt_get_delay();
    struct uart_funcs * uart = cpe_get_info()->uart;

    console_printf(
        uart,
        "Heartbeat LED toggle delay: %d milliseconds\r\n", val);
}

static void cmd_hrtbt_set_delay(void * x)
{
    (void)x;
    char * end;
    struct cpe_info * info = cpe_get_info();
    struct uart_funcs * uart = info->uart;

    //snprintf(line, info->token[3].len, "%s", info->token[3].start);
    const long val = strtol(info->token[3].start, &end, 10);
    if (1 > val)
    {
        console_printf(
            uart,
            "Heartbeat delay must be positive.\r\n", val);
    }
    else
    {
        hrtbt_set_delay((unsigned)val);
        console_printf(uart,
            "Heartbeat delay updated.\r\n", val);
    }
}

static void cmd_hrtbt_help(void * x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "hrtbt command actions\r\n\r\n");
    console_printf(uart,
        "get - Retrieve the current value for a setting\r\n");
    console_printf(uart,
        "set - Institute a given value for a setting\r\n");
}

static void cmd_hrtbt_get_set_help(void * x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "hrtbt command settings\r\n\r\n");
    console_printf(uart,
        "delay - This is the time it takes for the heartbeat LED to change");
    console_printf(uart,
        "states in\r\n");
    console_printf(uart,
        "        milliseconds\r\n");
}

static void cmd_hrtbt_set_delay_help(void * x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "This is the duration in milliseconds after which the LED state ");
    console_printf(uart,
        "toggles.\r\n");
    console_printf(uart,
        "It needs to be an integer.\r\n");
}

END_C_DECLS
