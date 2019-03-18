#include <stdio.h>
#include <stdint.h>

#include "mac_lstnr_wifi.h"

void mac_to_str(const uint8_t *mac, char *buf)
{
    sprintf(
        buf,
        "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}