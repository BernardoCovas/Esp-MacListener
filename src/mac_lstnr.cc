#include <string.h>
#include <malloc.h>
#include <stdint.h>

#include <esp_wifi.h>

#include "mac_lstnr.h"
#include "mac_lstnr_wifi.h"
#include "mac_lstnr_config.h"

mac_listnr_t *mac_listnr_init()
{
    mac_listnr_known_t known[] = MAC_LISTNR_KNOWN_CONFIG;

    mac_listnr_t *listener = (mac_listnr_t *) malloc(sizeof(mac_listnr_known_t));
    
    listener->num_known = sizeof(known) / sizeof(mac_listnr_known_t);
    listener->known = (mac_listnr_known_t *) malloc(sizeof(mac_listnr_known_t) * listener->num_known);
    
    memcpy(listener->known, known, sizeof(known));

    for (int i=0; i<listener->num_known; i++)
    {
        listener->known[i]._hits =  0;
        listener->known[i]._secs = -1;
    }

    return listener;
}

int mac_listnr_filter_pkt(mac_listnr_t *listener, void *buff)
{
	const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *) buff;
	const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *) ppkt->payload;
	const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;
	const uint8_t *addr = hdr->s_addr;

	char mac[MAC_LSTNR_MACSTR_LEN];
	mac_to_str(addr, mac);
	
    int r = mac_listener_find_mac(listener, hdr->r_addr);
    if (r >= 0) return r;
    int s = mac_listener_find_mac(listener, hdr->s_addr);
    if (s >= 0) return s;
    
    return -1;
}

void mac_listnr_deinit(mac_listnr_t *listener)
{
    free(listener->known);
    free(listener);
}

int mac_listener_find_mac(mac_listnr_t *listener, const uint8_t *mac)
{
    char _mac[MAC_LSTNR_MACSTR_LEN];
    mac_to_str(mac, _mac);

    for (int i=0; i<listener->num_known; i++)
        if (strcmp(listener->known[i].mac, _mac) == 0)
            return i;

    return -1;
}