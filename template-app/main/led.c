#include "led.h"
#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

/**
 * Private Function Declaractions
 */
static void toggleLED(LEDObject* pLEDObject);

/**
 * Public Function Definitions
 */

/**
 * Setup LED task
 */
void setupLED(TaskHandle_t* pTaskHandle, LEDObject* pLEDObject) {
  esp_err_t errorStatus = ESP_OK;

  /* Set the GPIO as a push/pull output */
  gpio_pad_select_gpio(pLEDObject->gpioPin);
  errorStatus = gpio_set_direction(pLEDObject->gpioPin, GPIO_MODE_OUTPUT);
  if (errorStatus) {
    printf("Something went wrong with gpio configuration\r\n");
  }

  // By default, toggle it to be true
  pLEDObject->ledOn = true;

  xTaskCreatePinnedToCore(
    toggleLED,        // Function
    "ToggleLED",      // Task Name
    8192,             // stack size (bytes)
    pLEDObject,       // parameter
    1,                // Task Priority, with higher numbers mean more important
    pTaskHandle,      // Task handle
    0                 // Core 0 or 1
  );
}

/**
 * Turn on LED
 */
void turnOnLED(LEDObject* pLEDObject) {
  // printf("Turn on LED\r\n");

  if (pLEDObject->ledOn) {
    gpio_set_level(pLEDObject->gpioPin, 1);
  }
}

/**
 * Turn off LED
 */
void turnOffLED(LEDObject* pLEDObject) {
  // printf("Turn off LED\r\n");
  gpio_set_level(pLEDObject->gpioPin, 0);
}

/**
 * Private Function Definitions
 */

/**
 * Infinite Loop of Toggling LED on and off
 */
static void toggleLED(LEDObject* pLEDObject) {
  for (;;) {
    
    turnOnLED(pLEDObject);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    
    turnOffLED(pLEDObject);
    vTaskDelay(1000/portTICK_PERIOD_MS);   
  }
}
