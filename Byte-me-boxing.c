#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 15

int main() {
    // Initialize USB serial output
    stdio_init_all();

    // Initialize GP15 as an output pin for the LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Give Windows 2 seconds to recognize the USB COM port after booting
    sleep_ms(2000);

    while (true) {
        // Turn LED ON and print message
        gpio_put(LED_PIN, 1);
        printf("LED is ON!\n");
        sleep_ms(1000);

        // Turn LED OFF and print message
        gpio_put(LED_PIN, 0);
        printf("LED is OFF!\n");
        sleep_ms(1000);
    }
}