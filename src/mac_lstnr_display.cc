#include <stdio.h>

#include "mac_lstnr_display.h"

void mac_lstnr_display_results(mac_listnr_t *listener)
{
    printf("\n-----------------------------------------");

    for (int i=0; i<listener->num_known; i++) {
        printf("\n%10s - %d hits, Lat seen: %ds.",
            listener->known[i].name,
            listener->known[i]._hits,
            listener->known[i]._secs
        );
    }

    printf("\n-----------------------------------------\n");
}
