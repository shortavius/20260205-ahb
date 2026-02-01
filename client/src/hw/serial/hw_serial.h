///
/// @file
///
/// @brief
/// Arduino Serial UART interface
///
/// @details
/// This is a helper module which deals with the Arduino Nano's UART, which is
/// named Serial.

#if !defined(_HW_SERIAL)
#define _HW_SERIAL

//
// Shared Includes
//
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

//
// Shared Structures / Enumerations / Type Definitions
//

//
// Shared Function Prototypes
//

/// @brief
/// Initialize CPU UART
///
/// @details
/// This function validates that the CPU has the required UART and prepares it
/// to be used in the rest of the program.
///
/// @param
/// baud [in] This indicates the speed of that the UART communicates at.
void
hw_serial_init_uart(unsigned long baud);

/// @brief
/// Get UART read function
///
/// @details
/// Get a pointer to the function that reads data from the UART.
size_t
(*hw_serial_get_read_fn(void)) (void * buff, size_t len);

/// @brief
/// Get UART write function
///
/// @details
/// Get a pointer to the function that writes data to the UART.
size_t
(*hw_serial_get_write_fn(void)) (const void * buff, size_t len);

//
// Shared Global Variables
//

END_C_DECLS

#endif
