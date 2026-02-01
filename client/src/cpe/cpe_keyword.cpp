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
static const char * cpe_keyword_list[CPE_NUM_KEYWORDS] = {
    "?",
    "get",
    "help",
    "hrtbt",
    "name",
    "ntwrk",
    "pass",
    "scan",
    "set",
    "ssid",
    "status",
    "delay",
    "connect",
    "ip",
    "ping"
};

static const cpe_token_keyword_t cpe_keyword_list_code[CPE_NUM_KEYWORDS] = {
    (cpe_token_keyword_t)CPE_KEYWORD_HELP,
    (cpe_token_keyword_t)CPE_KEYWORD_GET,
    (cpe_token_keyword_t)CPE_KEYWORD_HELP,
    (cpe_token_keyword_t)CPE_KEYWORD_HRTBT,
    (cpe_token_keyword_t)CPE_KEYWORD_NAME,
    (cpe_token_keyword_t)CPE_KEYWORD_NTWRK,
    (cpe_token_keyword_t)CPE_KEYWORD_PASS,
    (cpe_token_keyword_t)CPE_KEYWORD_SCAN,
    (cpe_token_keyword_t)CPE_KEYWORD_SET,
    (cpe_token_keyword_t)CPE_KEYWORD_SSID,
    (cpe_token_keyword_t)CPE_KEYWORD_STATUS,
    (cpe_token_keyword_t)CPE_KEYWORD_DELAY,
    (cpe_token_keyword_t)CPE_KEYWORD_CONNECT,
    (cpe_token_keyword_t)CPE_KEYWORD_IP,
    (cpe_token_keyword_t)CPE_KEYWORD_PING
};

//
// Code
//
const char * cpe_keyword_get(int sel)
{
    if (sel >= (int)CPE_NUM_KEYWORDS) { return NULL; }

    return cpe_keyword_list[sel];
}

cpe_token_keyword_t cpe_keyword_code_get(int sel)
{
    if (sel >= (int)CPE_NUM_KEYWORDS)
    {
        return (cpe_token_keyword_t)CPE_KEYWORD_MAX;
    }

    return cpe_keyword_list_code[sel];
}

END_C_DECLS
