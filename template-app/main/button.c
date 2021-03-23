#include "button.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>

/**
 * Private Function Declaractions
 */
static void buttonHandler(Button* pButton);
static void buttonEvent(Button* pButton);

/**
 * Public Function Definitions
 */

/**
 * Setup Button
 */
void setupButton(TaskHandle_t* pTaskHandle, Button* pButton) {
  gpio_config_t gpioConfig;
	// gpioConfig.pin_bit_mask = pButton->gpioPinMask;
  gpioConfig.pin_bit_mask = GPIO_SEL_15; // need to fix this asap!
	gpioConfig.mode         = GPIO_MODE_INPUT;
	gpioConfig.pull_up_en   = GPIO_PULLUP_DISABLE;
	gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
	gpioConfig.intr_type    = GPIO_INTR_POSEDGE;

  esp_err_t errorStatus = ESP_OK;

	errorStatus = gpio_config(&gpioConfig);
  if (errorStatus) {
    printf("Something went wrong with gpio config\r\n");
  }
  errorStatus = gpio_install_isr_service(0);
  if (errorStatus) {
    printf("Something went wrong with isr service\r\n");
  }

	errorStatus = gpio_isr_handler_add(pButton->gpioPin, buttonHandler, pButton);
  if (errorStatus) {
    printf("Something went wrong with isr handler\r\n");
  }

  xTaskCreatePinnedToCore(
    buttonEvent,          // Function
    "Button Event",       // Task Name
    8192,                 // stack size (bytes)
    pButton,              // parameter
    2,                    // Task Priority, with higher numbers mean more important
    pTaskHandle,          // Task handle
    0                     // Core 0 or 1
  );
}

/**
 * Button Event
 */
static void buttonEvent(Button* pButton) {
  for (;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Interrupt may interfere with this but should not happen in most cases
    if (pButton->pressed) {
      pButton->pressed = false;
      printf("Number of button presses: %u\r\n", pButton->numKeyPresses);
      pButton->numKeyPresses = 0;
    }
  }
}

/**
 * Private Function Definitions
 */
static void buttonHandler(Button* pButton) {
  pButton->pressed = true;

  if (pButton->numKeyPresses < UINT32_MAX) {
    pButton->numKeyPresses++;
  }
}
