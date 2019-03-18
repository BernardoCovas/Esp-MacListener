#include <Arduino.h>
#include <WiFi.h>

#include <esp_wifi.h>

#include "mac_lstnr.h"
#include "mac_lstnr_wifi.h"
#include "mac_lstnr_display.h"

#ifdef CONFIG_BLINK_GPIO
#define	LED_GPIO_PIN CONFIG_BLINK_GPIO
#else
#define	LED_GPIO_PIN GPIO_NUM_2 // Default for esp32
#endif

#define BLINK_PATTERN_NEW_PACKET_BADADDR 3, 33
#define BLINK_PATTERN_NEW_PACKET 1, 100
#define BLINK_PATTERN_NEW_ADDR 2, 50

void rx_callback(void *buff, wifi_promiscuous_pkt_type_t type);
void blinkPattern(uint16_t num_blinks, uint32_t delay_ms);
void button_interrupt(void);

mac_listnr_t *listener;
int curr_channel = 0;
bool screen_lit = false;

void setup() {
	listener = mac_listnr_init();
	mac_listnr_wifi_init(&rx_callback);

	for (int i=0; i<listener->num_known; i++) {
		printf("Known mac: %s\n", listener->known[i].mac);
	}

	gpio_set_direction(LED_GPIO_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_0, GPIO_PULLUP_ONLY);

	attachInterrupt(digitalPinToInterrupt(GPIO_NUM_0), &button_interrupt, FALLING);
}

void loop()
{
	esp_wifi_set_channel(curr_channel, WIFI_SECOND_CHAN_NONE);
	
	if (screen_lit) mac_lstnr_display_results(listener);
	else /* clear display */ ;

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
		blinkPattern(BLINK_PATTERN_NEW_PACKET_BADADDR);
	}
}

void blinkPattern(uint16_t num_blinks, uint32_t delay_ms) {
	
	for (int i=0; i<num_blinks; i++) {
		gpio_set_level(LED_GPIO_PIN, 1);
		vTaskDelay(delay_ms / portTICK_PERIOD_MS);
		gpio_set_level(LED_GPIO_PIN, 0);
		vTaskDelay(delay_ms / portTICK_PERIOD_MS);
	}

}

void button_interrupt()
{
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	
	if (interrupt_time - last_interrupt_time > 200) 
	{
		screen_lit = !screen_lit;
	}

 	last_interrupt_time = interrupt_time;
}