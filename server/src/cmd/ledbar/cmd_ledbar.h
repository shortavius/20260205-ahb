///
/// @file
///
/// @brief
/// Led bar command processing (header)
///
/// @details
/// This module handles the CLI commands that interface with the LED bar. This
/// allows for setting the state of each segment in the LED bar along with
/// whether that segment is blinking or state along with the delay during the
/// blinking state.

#if !defined(_CMD_LEDBAR_H)
#define _CMD_LEDBAR_H

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
void
cmd_ledbar_root_help(void);

uint8_t
cmd_ledbar_syntax(void);

//
// Shared Global Variables
//

END_C_DECLS

#endif
