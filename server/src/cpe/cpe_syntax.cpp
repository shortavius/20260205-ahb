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
#include "cpe.h"
#include "../cmd/hrtbt/cmd_hrtbt.h"
#include "../cmd/help/cmd_help.h"
#include "../cmd/ntwrk/cmd_ntwrk.h"

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

//
// Local Global Variables
//

//
// Code
//
void cpe_checking_syntax(void)
{
    struct cpe_info * info = cpe_get_info();
    uint8_t valid_syntax = 0x0;

    // If nothing has been entered, then do nothing
    if (((uint8_t)1 == info->token_cnt) &&
        ((size_t)0 == info->token[0].len))
    {
        console_process_command_done(info->uart);
        info->state = (cpe_state_t)CPE_STATE_WAITING_FOR_DATA;

        return;
    }

    // The first item should be the main command
    if ((cpe_token_keyword_t)CPE_KEYWORD_UNDEFINED == info->token[0].kyw)
    {
        console_printf(info->uart, "[ERR: Command not found ]");
        console_process_command_done(info->uart);
        info->state = (cpe_state_t)CPE_STATE_WAITING_FOR_DATA;

        return;
    }

    if (!valid_syntax) { valid_syntax = cmd_help_syntax(); }
    if (!valid_syntax) { valid_syntax = cmd_hrtbt_syntax(); }
    if (!valid_syntax) { valid_syntax = cmd_ntwrk_syntax(); }

    // Check various syntax
    if ((uint8_t)0x0 == valid_syntax)
    {
        console_printf(info->uart, "[ERR: Syntax ]");
    }

    info->state = (cpe_state_t)CPE_STATE_WAITING_FOR_DATA;
    console_process_command_done(info->uart);
}

END_C_DECLS
