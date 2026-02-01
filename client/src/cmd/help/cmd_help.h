///
/// @file
///
/// @brief
///
///
/// @details
///

#if !defined(_CMD_HELP_H)
#define _CMD_HELP_H

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

//
// Shared Function Prototypes
//
uint8_t cmd_help_syntax(void);

//
// Shared Global Variables
//

END_C_DECLS

#endif
