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
#include "../ledbar/ledbar.h"

#include <string.h>

#include <WiFiNINA.h>
#include <WiFiUdp.h>
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
#define UDP_RX_PACKET_SIZE                  1024

//
// Local Structures / Enumerations / Type Definitions
//
enum udp_srvr_status
{
    NTWRK_UDP_SRVR_STOPPED,
    NTWRK_UDP_SRVR_RUNNING,
    NTWRK_UDP_SRVR_MAX
};

struct udp_rxtx_ports
{
    uint16_t port_rx;
    uint16_t port_tx;
};

struct ntwrk_info
{
    struct ssid_info ssid;
    uint8_t status;
    enum udp_srvr_status srvr_status;
    struct udp_rxtx_ports ports;
};

WiFiUDP udp;

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

    if ((enum udp_srvr_status)NTWRK_UDP_SRVR_STOPPED == nw_info.srvr_status)
    {
        return;
    }

    int pkt_size = udp.parsePacket();
    if (pkt_size)
    {
        char pkt_buff[UDP_RX_PACKET_SIZE] = { 0 };
        int pkt_len = 0;
        Serial.println("Got a packet");

        while (pkt_len < pkt_size)
        {
            pkt_len += udp.read(
                &pkt_buff[pkt_len], ((int)UDP_RX_PACKET_SIZE - pkt_len));
        }

        if (pkt_len > 0)
        {
            pkt_buff[pkt_len] = '\0';
            StaticJsonDocument<JSON_MEMORY_SIZE> doc;
            DeserializationError err = deserializeJson(doc, pkt_buff);

            if (DeserializationError::Ok == err)
            {
                const int led = (int)doc["led"];
                const char * mode = doc["mode"];
                const int delay = (int)doc["delay"];

                if (0 < delay)
                {
                    ledbar_set_blink_rate_msecs(led, (uint64_t)delay);
                }

                if (!(strncasecmp(mode, "blink", strlen("blink"))))
                {
                    ledbar_set_led_mode(
                        led, (enum led_commands)LED_COMMAND_BLINK);
                }
                else if (!(strncasecmp(mode, "static_on", strlen("static_on"))))
                {
                    ledbar_set_led_mode(
                        led, (enum led_commands)LED_COMMAND_ON);
                }
                else
                {
                    ledbar_set_led_mode(
                        led, (enum led_commands)LED_COMMAND_OFF);
                }
            }
        }
    }
}

struct ssid_info *
ntwrk_cfg(void)
{
    nw_info.status = WiFi.status();
    nw_info.ports.port_rx = 0u;
    nw_info.ports.port_tx = 0u;
    nw_info.srvr_status = (enum udp_srvr_status)NTWRK_UDP_SRVR_STOPPED;
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

void
ntwrk_udp_srvr_start(void)
{
    if ((enum udp_srvr_status)NTWRK_UDP_SRVR_RUNNING == nw_info.srvr_status)
    {
        return;
    }
    udp.begin(nw_info.ports.port_rx);
    nw_info.srvr_status = (enum udp_srvr_status)NTWRK_UDP_SRVR_RUNNING;
}

END_C_DECLS
