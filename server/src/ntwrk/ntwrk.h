///
/// @file
///
/// @brief
///
///
/// @details
///

#if !defined(_NETWRK_H)
#define _NETWRK_H

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
#define SSID_NAME_MAX_NUM_CHARACTERS        25
#define SSID_PASS_MAX_NUM_CHARACTERS        25

//
// Shared Structures / Enumerations / Type Definitions
//
struct ssid_info
{
    uint8_t name[SSID_NAME_MAX_NUM_CHARACTERS + 1u];
    uint8_t pass[SSID_PASS_MAX_NUM_CHARACTERS + 1u];
};

//
// Shared Function Prototypes
//
void
ntwrk_action(struct ssid_info * ssid);

struct ssid_info *
ntwrk_cfg(void);

uint8_t
ntwrk_get_wifi_status(void);

char *
ntwrk_get_ssid_name(void);

char *
ntwrk_get_ssid_pass(void);

uint16_t
ntwrk_get_udp_rx_port(void);

void
ntwrk_set_udp_rx_port(uint16_t port);

uint16_t
ntwrk_get_udp_tx_port(void);

void
ntwrk_set_udp_tx_port(uint16_t port);

void
ntwrk_udp_srvr_start(void);

//
// Shared Global Variables
//

END_C_DECLS

#endif
