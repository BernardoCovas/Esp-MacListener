#ifndef _MAC_LSTNR_WIFI_H
#define _MAC_LSTNR_WIFI_H

#define MAC_LSTNR_MACSTR_LEN 18

#include "stdint.h"


typedef struct {
	unsigned frame_ctrl:16;
	unsigned duration_id:16;
	uint8_t r_addr[6]; /* receiver address */
	uint8_t s_addr[6]; /* sender address */
	uint8_t f_addr[6]; /* filtering address */
	unsigned sequence_ctrl:16;
	uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
	wifi_ieee80211_mac_hdr_t hdr;
	uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;


void mac_to_str(const uint8_t *mac, char *buf);


#endif /* _MAC_LSTNR_WIFI_H */