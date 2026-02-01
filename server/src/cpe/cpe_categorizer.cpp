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

//
// Local Global Variables
//

//
// Code
//
void cpe_token_categorizer(void)
{
    int idx;
    struct cpe_info * info = cpe_get_info();

    for (idx = (int)0; idx < info->token_cnt; idx++)
    {
        if (cpe_token_is_integer(&info->token[idx]))
        {
            info->token[idx].cat = (cpe_token_cat_t)CPE_TOKEN_CAT_INTEGER;
        }
        else if (cpe_token_is_decimal(&info->token[idx]))
        {
            info->token[idx].cat = (cpe_token_cat_t)CPE_TOKEN_CAT_DECIMAL;
        }
        else if (cpe_token_is_keyword(&info->token[idx]))
        {
            info->token[idx].cat = (cpe_token_cat_t)CPE_TOKEN_CAT_KEYWORD;
        }
        else
        {
            info->token[idx].cat = (cpe_token_cat_t)CPE_TOKEN_CAT_STRING;
        }
    }

    info->state = (cpe_state_t)CPE_STATE_CHECKING_SYNTAX;
}

END_C_DECLS
