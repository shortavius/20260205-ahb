///
/// @file
///
/// @brief
/// Command processing engine main header
///
/// @details
/// This is the main header file for the command processing engine. This engine
/// is responsible for processing the command line fed to it.

#if !defined(_CPE_H)
#define _CPE_H

//
// Shared Includes
//
#include "../console/console.h"

#include <stdint.h>
#include <stddef.h>

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

#define CPE_MAX_LINE_SZ                     128u
#define CPE_MAX_TOKENS_PER_LINE             14
#define CPE_NUM_KEYWORDS                    15u

//
// Shared Structures / Enumerations / Type Definitions
//

typedef enum
{
    CPE_STATE_WAITING_FOR_DATA,
    CPE_STATE_LOOKING_FOR_TOKENS,
    CPE_STATE_CATEGORIZING_TOKENS,
    CPE_STATE_CHECKING_SYNTAX,
    CPE_STATE_MAX_STATE
} cpe_state_t;

typedef enum
{
    CPE_TOKEN_CAT_UNDEFINED,
    CPE_TOKEN_CAT_INTEGER,
    CPE_TOKEN_CAT_DECIMAL,
    CPE_TOKEN_CAT_KEYWORD,
    CPE_TOKEN_CAT_STRING,
    CPE_TOKEN_CAT_MAX
} cpe_token_cat_t;

typedef enum
{
    CPE_KEYWORD_UNDEFINED,
    CPE_KEYWORD_CONNECT,
    CPE_KEYWORD_DELAY,
    CPE_KEYWORD_GET,
    CPE_KEYWORD_HELP,
    CPE_KEYWORD_HRTBT,
    CPE_KEYWORD_IP,
    CPE_KEYWORD_NAME,
    CPE_KEYWORD_NTWRK,
    CPE_KEYWORD_PASS,
    CPE_KEYWORD_PING,
    CPE_KEYWORD_SCAN,
    CPE_KEYWORD_SET,
    CPE_KEYWORD_SSID,
    CPE_KEYWORD_STATUS,
    CPE_KEYWORD_MAX
} cpe_token_keyword_t;

struct cpe_token {
    char * start;
    size_t len;
    cpe_token_cat_t cat;
    cpe_token_keyword_t kyw;
};

struct cpe_info {
    cpe_state_t state;
    char * proc_str;
    struct uart_funcs * uart;
    struct cpe_token token[CPE_MAX_TOKENS_PER_LINE];
    uint8_t token_cnt;
};

struct cpe_syntax_tkn {
    cpe_token_cat_t cat;
    cpe_token_keyword_t kyw;
};

struct cpe_cmd_syntax {
    uint8_t count;
    const struct cpe_syntax_tkn * syntax_tkns;
    void (*action)(void * x);
};

//
// Shared Function Prototypes
//
void cpe_cfg(struct uart_funcs * uart);
void cpe_action(void);
void cpe_set_processing_buffer(const uint8_t * const new_buffer);
char * cpe_skip_spcs(char * const st);
struct cpe_info * cpe_get_info(void);
int cpe_token_is_integer(struct cpe_token * tkn);
int cpe_token_is_decimal(struct cpe_token * tkn);
int cpe_token_is_keyword(struct cpe_token * tkn);
uint8_t cpe_cmp_syntax_tkns(struct cpe_syntax_tkn a,
                            struct cpe_syntax_tkn b);
void cpe_token_locate(void);
void cpe_token_categorizer(void);
const char * cpe_keyword_get(int sel);
cpe_token_keyword_t cpe_keyword_code_get(int sel);
void cpe_checking_syntax(void);

//
// Shared Global Variables
//

END_C_DECLS

#endif
