#include <Arduino.h>
#include <WiFi.h>

#include <esp_wifi.h>

#include "mac_lstnr_wifi.h"
#include "mac_listnr.h"

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
	
	vTaskDelay(pdMS_TO_TICKS(300));
	
	if (++curr_channel > 13)
		curr_channel = 0;
}

void rx_callback(void *buff, wifi_promiscuous_pkt_type_t type)
{
	int index = mac_listnr_filter_pkt(listener, buff);
	if (index >= 0) printf("%s: %s\n", listener->known[index].name, listener->known[index].mac);
}