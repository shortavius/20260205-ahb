///
/// @file
///
/// @brief
/// Basic UART console implementation header
///
/// @details
/// This is the header file for the module that is responsible for handling the
/// transmission and reception of data via the console UART. It implements a
/// basic console that the user is able to interact with.

#if !defined(_CONSOLE_H)
#define _CONSOLE_H

//
// Shared Includes
//
#include <stddef.h>
#include <stdarg.h>

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

/// @brief
/// Escape buffer size
///
/// @details
/// This is the number of bytes to be set aside for the escape buffer.
#define CONSOLE_ESC_BUFF_SZ                 8u

/// @brief
/// Max number of characters per line
///
/// @details
/// This defines the number of character that will be allowed in a single
/// command line.
#define CONSOLE_MAX_LINE_SZ                 128u

/// @brief
/// Max number of characters to be read
///
/// @details
/// This defines the maximum number of characters that could be returned form
/// the UART in a single read.
#define CONSOLE_READ_BUFF_SZ                32u

/// @brief
/// VPRINTF buffer size
///
/// @details
/// This is the number of bytes set aside for the vprintf buffer.
#define CONSOLE_VPRINTF_BUFF_SZ             512u

//
// Shared Structures / Enumerations / Type Definitions
//

/// @brief
/// UART commands
///
/// @details
/// This structure contains the functions that interact with the specific
/// device's UART port.
struct uart_funcs
{
    /// @brief
    /// Pointer to function to read characters from UART
    size_t (*read_fn)(void * buff, size_t len);
    /// @brief
    /// Pointer to function to write characters to UART
    size_t (*write_fn)(const void * buff, size_t len);
};

//
// Shared Function Prototypes
//

/// @brief
/// The console action function
///
/// @details
/// This function allows the console module to perform any tasks that it needs
/// to perform.
void
console_action(struct uart_funcs * uart);

/// @brief
/// Initialize console
///
/// @details
/// This function initializes the console structures and prepares them for use.
///
/// @param [in] read_fn
/// This is a pointer to a function that reads data from a UART
///
/// @param [in] write_fn
/// This is a pointer to a function that writes data to a UART
///
/// @return
/// A pointer to the created uart_funcs structure
struct uart_funcs *
console_init(
    size_t (*read_fn)(void *, size_t),
    size_t (*write_fn)(const void *, size_t));

/// @brief
/// Show main prompt
///
/// @details
/// This functions displays the console's prompt.
void
console_show_prompt(struct uart_funcs * uart);

/// @brief
/// Send va_list data
///
/// @details
/// Send a va_list to the specified UART
///
/// @param [in] uart
/// A pointer to the uart functions
///
/// @param [in] fmt
/// Standard C formatting string
///
/// @param [in] ap
/// The va_list for the format string
///
/// @return
/// The number of character sent to the UART
size_t
console_vprintf(struct uart_funcs * uart, const char * fmt, va_list ap);

/// @brief
/// Send formatted string
///
/// @details
/// Send a formatted string of character to the specified UART
///
/// @param [in] uart
/// A pointer to the uart functions
///
/// @param [in] fmt
/// Standard C formatting string
///
/// @return
/// The number of character sent to the UART
size_t
console_printf(struct uart_funcs * uart, const char *fmt, ...)
	__attribute__ ((format (printf, 2, 3)));

/// @brief
/// Signal command processing is over
///
/// @details
/// This function tells the console that the processing of the supplied command
/// is finished and to start building up the next command line.
///
/// @param [in] uart
/// A pointer to the uart functions
void
console_process_command_done(struct uart_funcs * uart);

//
// Shared Global Variables
//

END_C_DECLS

#endif
