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
#include "ntwrk.h"
#include "../misc/macros.h"

#include <string.h>

#include <WiFiNINA.h>

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
struct udp_rxtx_ports
{
    uint16_t port_rx;
    uint16_t port_tx;
};

struct ntwrk_info
{
    struct ssid_info ssid;
    uint8_t status;
    struct udp_rxtx_ports ports;
};

//
// Local Function Prototypes
//

//
// Local Global Variables
//
struct ntwrk_info nw_info = {
    .ssid =
    {
        .name = { 0u },
        .pass = { 0u }
    },
    .status = WL_NO_MODULE
};

//
// Code
//
void
ntwrk_action(struct ssid_info * ssid)
{
    nw_info.status = WiFi.status();
}

struct ssid_info *
ntwrk_cfg(void)
{
    nw_info.status = WiFi.status();
    (void)memset(nw_info.ssid.name, '\0', COUNT_OF(nw_info.ssid.name));
    (void)memset(nw_info.ssid.pass, '\0', COUNT_OF(nw_info.ssid.pass));

    return &nw_info.ssid;
}

uint8_t
ntwrk_get_wifi_status(void)
{
    return nw_info.status;
}

char *
ntwrk_get_ssid_name(void)
{
    return (char *)nw_info.ssid.name;
}

char *
ntwrk_get_ssid_pass(void)
{
    return (char *)nw_info.ssid.pass;
}

uint16_t
ntwrk_get_udp_rx_port(void)
{
    return nw_info.ports.port_rx;
}

void
ntwrk_set_udp_rx_port(uint16_t port)
{
    nw_info.ports.port_rx = port;
}

uint16_t
ntwrk_get_udp_tx_port(void)
{
    return nw_info.ports.port_tx;
}

void
ntwrk_set_udp_tx_port(uint16_t port)
{
    nw_info.ports.port_tx = port;
}

END_C_DECLS
