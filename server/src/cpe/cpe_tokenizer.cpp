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
static void cpe_token_init(void);

//
// Local Global Variables
//

//
// Code
//
void cpe_token_locate(void)
{
    struct cpe_info * info = cpe_get_info();
    char * st = &info->proc_str[0];
    char * ts = &info->proc_str[0];
    char * te = &info->proc_str[0];
    uint8_t flag = 0x1;
    uint16_t token_id = 0;
    char tmpbuf[80] = { 0 };

    // Prepare the tokens
    cpe_token_init();

    // Jump over any initial spaces
    ts = cpe_skip_spcs(ts);

    // Parse through the string and locate places where there are spaces
    while (flag)
    {
        while ((*te != ' ') && ((te - st) < CPE_MAX_LINE_SZ) && (flag == 0x01))
        {
            if ((*te == '\0') || ((te - st) > CPE_MAX_LINE_SZ))
            {
                flag = 0x0;
            }
            else
            {
                te++;
            }
        }

        info->token[info->token_cnt].start = ts;
        info->token[info->token_cnt++].len = (size_t)(te - ts);

        te = cpe_skip_spcs(te);
        ts = te;
    }

    info->state = (cpe_state_t)CPE_STATE_CATEGORIZING_TOKENS;
}

static void cpe_token_init(void)
{
    struct cpe_info * info = cpe_get_info();
    int token_id;

    for (token_id = 0; token_id < (int)CPE_MAX_TOKENS_PER_LINE; token_id++)
    {
        info->token[token_id].start = NULL;
        info->token[token_id].len = (size_t)0;
        info->token[token_id].cat = (cpe_token_cat_t)CPE_TOKEN_CAT_UNDEFINED;
        info->token[token_id].kyw = (cpe_token_keyword_t)CPE_KEYWORD_UNDEFINED;
    }

    info->token_cnt = (uint8_t)0u;
}

END_C_DECLS
