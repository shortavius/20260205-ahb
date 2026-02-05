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

#include <WiFi.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>

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
#define JSON_MEMORY_SIZE                    1024
#define UDP_TX_PACKET_SIZE                  1024

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
    WiFiUDP udp;
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

String
ntwrk_build_pkt(int led, const char * mode, int delay)
{
    String pkt;

    StaticJsonDocument<JSON_MEMORY_SIZE> doc;
    doc["led"] = led;
    doc["mode"] = mode;
    doc["delay"] = delay;
    serializeJson(doc, pkt);

    return pkt;
}

void
ntwrk_send_pkt(IPAddress ip, String pkt)
{
    char tx_buf[UDP_TX_PACKET_SIZE] = { 0 };

    pkt.toCharArray(tx_buf, (unsigned)UDP_TX_PACKET_SIZE);
    nw_info.udp.beginPacket(ip, nw_info.ports.port_tx);
    nw_info.udp.write(tx_buf, pkt.length());
    nw_info.udp.endPacket();
}

void
ntwrk_udp_srvr_start(void)
{
    nw_info.udp.begin((uint16_t)1234);
}



END_C_DECLS
