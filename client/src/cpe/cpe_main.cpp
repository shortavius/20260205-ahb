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

#include <string.h>

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
static void cpe_clr_buffer(void);
static void cpe_display_state(void);

//
// Local Global Variables
//
static char cpe_cmd_str[CPE_MAX_LINE_SZ + 1] = { 0u };

static struct cpe_info cmd_proc_state = {
    .state = CPE_STATE_MAX_STATE,
    .proc_str = NULL,
    .uart = NULL,
    .token = { 0 },
    .token_cnt = (uint8_t)0u,
};

//
// Code
//
void cpe_cfg(struct uart_funcs * uart)
{
    // Clear the command processing buffer
    cpe_clr_buffer();

    // Initialize the command processing engine state
    cmd_proc_state.state = CPE_STATE_WAITING_FOR_DATA;
    cmd_proc_state.proc_str = &cpe_cmd_str[0];
    cmd_proc_state.uart = uart;
}

void cpe_action(void)
{
    switch (cmd_proc_state.state)
    {
        case CPE_STATE_LOOKING_FOR_TOKENS:
            cpe_token_locate();
            break;
        case CPE_STATE_CATEGORIZING_TOKENS:
            cpe_token_categorizer();
            break;
        case CPE_STATE_CHECKING_SYNTAX:
            cpe_checking_syntax();
            break;
        default:
            break;
    }
}

void cpe_set_processing_buffer(const uint8_t * const new_buffer)
{
    if (CPE_STATE_WAITING_FOR_DATA == cmd_proc_state.state)
    {
        cpe_clr_buffer();
        memcpy((void *)cmd_proc_state.proc_str,
               (void *)new_buffer,
               CPE_MAX_LINE_SZ);
        cmd_proc_state.state = (cpe_state_t)CPE_STATE_LOOKING_FOR_TOKENS;
    }
}

char * cpe_skip_spcs(char * const st)
{
    char * en = st;
    while ((*en == ' ') && (*en != '\0'))
        en++;

    return en;
}

struct cpe_info * cpe_get_info(void)
{
    return &cmd_proc_state;
}

int cpe_token_is_integer(struct cpe_token * tkn)
{
    int rv = (int)0;
    size_t tmp = (size_t)0;
    char buff[CPE_MAX_LINE_SZ + 1] = { 0 };

    (void)memcpy((void *)buff, (void *)tkn->start, tkn->len);
    tmp = strspn(buff, "0123456789");
    if (tkn->len == tmp) { rv = (int)1; }

    return rv;
}

int cpe_token_is_decimal(struct cpe_token * tkn)
{
    int rv = (int)0;
    size_t tmp = (size_t)0;
    char buff[CPE_MAX_LINE_SZ + 1] = { 0 };

    (void)memcpy((void *)buff, (void *)tkn->start, tkn->len);
    tmp = strspn(buff, "0123456789.");
    if (tkn->len == tmp) { rv = (int)1; }

    return rv;
}

int cpe_token_is_keyword(struct cpe_token * tkn)
{
    int rv = (int)0;
    int idx;
    const char * kyw = NULL;

    for (idx = (int)0; idx < CPE_NUM_KEYWORDS; idx++)
    {
        kyw = cpe_keyword_get(idx);
        if (!(strncasecmp(kyw, tkn->start, tkn->len)))
        {
            tkn->kyw = cpe_keyword_code_get(idx);
            rv = (int)1;
            break;
        }
    }

    return rv;
}

uint8_t cpe_cmp_syntax_tkns(struct cpe_syntax_tkn a,
                            struct cpe_syntax_tkn b)
{
    uint8_t rv = 0;

    if ((a.cat == b.cat) && (a.kyw == b.kyw))
        rv = 1;

    return rv;
}

static void cpe_clr_buffer(void)
{
    (void)memset((void *)&cpe_cmd_str[0],
                 '\0',
                 (sizeof(char) * CPE_MAX_LINE_SZ));
}

END_C_DECLS
