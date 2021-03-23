#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>

typedef struct {
  gpio_num_t gpioPin;
  uint8_t gpioPinMask;
  uint32_t numKeyPresses;
  bool pressed;
}Button;

void setupButton(TaskHandle_t* pTaskHandle, Button* pButton);

#endif // BUTTON_H
