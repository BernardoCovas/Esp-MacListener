#include <Arduino.h>
#include <WiFi.h>

#include <esp_wifi.h>

#include "mac_lstnr.h"
#include "mac_lstnr_wifi.h"
#include "mac_lstnr_display.h"

void rx_callback(void *buff, wifi_promiscuous_pkt_type_t type);
mac_listnr_t *listener;
int curr_channel = 0;


void setup() {

	listener = mac_listnr_init();
	mac_listnr_wifi_init(&rx_callback);

	for (int i=0; i<listener->num_known; i++) {
		printf("Known mac: %s\n", listener->known[i].mac);
	}

}

void loop()
{
	esp_wifi_set_channel(curr_channel, WIFI_SECOND_CHAN_NONE);
	mac_lstnr_display_results(listener);

	vTaskDelay(pdMS_TO_TICKS(1000));

	for (int i=0; i<listener->num_known; i++)
		if (listener->known[i]._secs >= 0)
			listener->known[i]._secs++;
	
	if (++curr_channel > 13)
		curr_channel = 0;
}

void rx_callback(void *buff, wifi_promiscuous_pkt_type_t type)
{
	int index = mac_listnr_filter_pkt(listener, buff);

	if (index >= 0) {
		listener->known[index]._hits  ++;
		listener->known[index]._secs = 0;
	}
}