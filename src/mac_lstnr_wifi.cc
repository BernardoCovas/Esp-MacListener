#include <stdio.h>
#include <stdint.h>

#include <esp_wifi.h>

#include "mac_lstnr_wifi.h"

void mac_to_str(const uint8_t *mac, char *buf)
{
    sprintf(
        buf,
        "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void mac_listnr_wifi_init(wifi_promiscuous_cb_t callback)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); 
	esp_wifi_init(&cfg); 
	
	esp_wifi_set_storage(WIFI_STORAGE_RAM); 
	esp_wifi_set_mode(WIFI_MODE_NULL); 
	esp_wifi_start(); 
	esp_wifi_set_promiscuous(true); 
	esp_wifi_set_promiscuous_rx_cb(callback); 
	esp_wifi_set_channel(0, WIFI_SECOND_CHAN_NONE);
}