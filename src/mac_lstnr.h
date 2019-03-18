#ifndef _MAC_LISTNR_H
#define _MAC_LISTNR_H

#include <stdint.h>

typedef struct
{
    const char *name;
    const char *mac;
    int _hits;
    int _secs;
} mac_listnr_known_t;

typedef struct
{
    mac_listnr_known_t *known;
    int num_known;
} mac_listnr_t;

mac_listnr_t *mac_listnr_init();
int mac_listener_find_mac(mac_listnr_t *listener, const uint8_t *mac);
int mac_listnr_filter_pkt(mac_listnr_t *listener, void *buff);
void mac_listnr_deinit(mac_listnr_t listener);


#endif /* _MAC_LISTNR_H */