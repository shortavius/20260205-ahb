///
/// @file
///
/// @brief
/// Arduino WiFi demo main
///
/// @details
/// This is the main Arduino project source for a demonstration of the WiFi
/// capabilities. It is intended to be given on 20251204 during a meetup.

//
// Local Includes
//
#include "src/hrtbt/hrtbt.h"
#include "src/hw/serial/hw_serial.h"
#include "src/cpe/cpe.h"
#include "src/ntwrk/ntwrk.h"

//
// Local Definitions
//

/// @brief
/// Defines the console baud rate
#define CONSOLE_BAUD                        115200

//
// Local Structures / Enumerations / Type Definitions
//
struct app_info
{
    struct uart_funcs * hw_uart;
    struct ssid_info * ssid;
};

//
// Local Function Prototypes
//

//
// Local Global Variables
//
struct app_info app = { 0 };

//
// Code
//

/// @brief
/// Arduino setup function
///
/// @details
/// This code is called only once and by the Arduino runtime prior to calling
/// the loop function. It will initialize the various CPU hardware that will be
/// used along with any software modules.
void setup()
{
    // Setup the heart beat
    hrtbt_cfg();

    // Setup the console
    hw_serial_init_uart((unsigned long)CONSOLE_BAUD);
    app.hw_uart = console_init(
        hw_serial_get_read_fn(),
        hw_serial_get_write_fn());

    // Setup the cpe
    cpe_cfg(app.hw_uart);

    // Setup the network
    app.ssid = ntwrk_cfg();
}

/// @brief
/// Arduino loop function
///
/// @details
/// This code is called after the setup function and runs indefinitely. It is
/// where the actual application code resides.
void loop()
{
    // Perform cpe actions
    cpe_action();

    // Perform console actions
    console_action(app.hw_uart);

    // Perform heart beat actions
    hrtbt_action();

    // Perform network actions
    ntwrk_action(app.ssid);
}
