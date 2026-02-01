///
/// @file
///
/// @brief
/// Arduino Serial UART interface
///
/// @details
/// This is a helper module which deals with the Arduino Nano's UART, which is
/// named Serial. 

//
// Local Includes
//
#include "hw_serial.h"

#include <Arduino.h>

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

size_t
hw_serial_read(void * buff, size_t len);

size_t
hw_serial_write(const void * buff, size_t len);

//
// Local Global Variables
//

//
// Code
//
void
hw_serial_init_uart(unsigned long baud)
{
    Serial.begin(baud);
    while(!Serial)
    {
        // If we get here, there is something seriously wrong.
        ;
    }
}

size_t 
(*hw_serial_get_read_fn(void)) (void * buff, size_t len)
{
    return hw_serial_read;
}

size_t 
(*hw_serial_get_write_fn(void)) (const void * buff, size_t len)
{
    return hw_serial_write;
}

size_t
hw_serial_read(void * buff, size_t len)
{
    const uint8_t * ptr = (uint8_t *)buff;
    int avail = 0;

    // Validate if there is anything to read
    if (0 >= (avail = Serial.available())) { return 0; }

    if (len < avail) { avail = len; }
    Serial.readBytes((char *)buff, avail);

    return (size_t)avail;
}

size_t
hw_serial_write(const void * buff, size_t len)
{
    const uint8_t * ptr = (uint8_t *)buff;
    size_t remaining = len;

    while (0 < remaining)
    {
        size_t write_capable = (size_t)Serial.availableForWrite();

        if (0 == write_capable) { continue; }

        if (remaining <= write_capable)
        {
            // Only send out what is remaining
            Serial.write(ptr, remaining);
            remaining = 0;
        }
        else
        {
            // Only write what is capable to be written
            Serial.write(ptr, write_capable);
            ptr += write_capable;
            remaining -= write_capable;
        }
    }

    return len;
}

END_C_DECLS
