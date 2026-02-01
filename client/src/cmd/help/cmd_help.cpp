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
#include "cmd_help.h"
#include "../../cpe/cpe.h"
#include "../hrtbt/cmd_hrtbt.h"
#include "../ntwrk/cmd_ntwrk.h"

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

//
// Local Structures / Enumerations / Type Definitions
//

//
// Local Function Prototypes
//
static void cmd_help(void * x);

//
// Local Global Variables
//
const static struct cpe_syntax_tkn syntax_cmd_hlp_tkns = {
    .cat = CPE_TOKEN_CAT_KEYWORD,
    .kyw = CPE_KEYWORD_HELP
};

const struct cpe_cmd_syntax syntax_cmd_help = {
    .count = 1,
    .syntax_tkns = &syntax_cmd_hlp_tkns,
    .action = cmd_help
};

//
// Code
//
uint8_t cmd_help_syntax(void)
{
    uint8_t rv = (uint8_t)1;
    struct cpe_info * info = cpe_get_info();

    if (syntax_cmd_help.count != info->token_cnt)
    {
        rv = 0;
    }

    if (rv)
    {
        int idx;
        for (idx = (int)0; idx < syntax_cmd_help.count; idx++)
        {
            struct cpe_syntax_tkn tmp = {
                .cat = info->token[idx].cat,
                .kyw = info->token[idx].kyw
            };

            if (!(cpe_cmp_syntax_tkns(syntax_cmd_help.syntax_tkns[idx], tmp)))
            {
                rv = 0;
                break;
            }
        }
    }

    if (rv)
    {
        uint8_t lvl = (uint8_t)0;
        syntax_cmd_help.action((void *)&lvl);
    }

    return rv;
}

static void cmd_help(void * x)
{
    struct uart_funcs * uart = cpe_get_info()->uart;
    (void)x;

    console_printf(uart,
        "Root help\r\n");
    console_printf(uart,
        "  ? or help - Display this message\r\n");
    cmd_hrtbt_root_help();
    cmd_ntwrk_root_help();
}

END_C_DECLS
