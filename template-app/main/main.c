#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_spi_flash.h>

#include "led.h"
#include "button.h"

#define GPIO_PIN_LED                2
#define GPIO_PIN_BUTTON             15

#define FIBONACCI_MAX_COEFFICIENT   20-1  // begins at 0. Actual max for UINT32 is 93th coefficient

/**
 * Recursively calculate fibonacci sequence's coefficient.
 * 
 * Deliberately inefficient.
 */
uint32_t fibonacciSequenceRecursive(uint32_t fibCoefficient) {

  // 0th and 1st coefficient is 0 and 1 respectively
  if (fibCoefficient <= 1) {
    return fibCoefficient;
  }

  return fibonacciSequenceRecursive(fibCoefficient-1) + fibonacciSequenceRecursive(fibCoefficient-2);
}

void app_main(void) {
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // for (int i = 10; i >= 0; i--) {
    //     printf("Restarting in %d seconds...\n", i);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    // printf("Restarting now.\n");
    // fflush(stdout);
    // esp_restart();

    LEDObject internalLED;
    internalLED.gpioPin = (gpio_num_t)GPIO_PIN_LED;

    Button buttonExample;
    buttonExample.gpioPin = (gpio_num_t)GPIO_PIN_BUTTON;
    buttonExample.numKeyPresses = 0;
    buttonExample.pressed = false;
    // buttonExample.gpioPinMask = GPIO_SEL_15; // Currently, not applying pin mask

    setupLED(NULL, &internalLED);
    setupButton(NULL, &buttonExample);

    uint32_t coefficient = 1;
    uint64_t number = 0;

    while(1) {
      // Calculate new Fibonacci coefficient number every second
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      number = fibonacciSequenceRecursive(coefficient);
      printf("Coefficient %u number: %llu\r\n", coefficient, number);
      coefficient++;

      if (coefficient == FIBONACCI_MAX_COEFFICIENT) {
        printf("Reached the limit\r\n");
        printf("Restarting coefficient\r\n");
        printf("\r\n");
        coefficient = 1;
      }
    }
}
