#ifndef __LED_H__
#define __LED_H__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

typedef struct {
  gpio_num_t gpioPin;
  bool ledOn;
}LEDObject;

void setupLED(TaskHandle_t* pTaskHandle, LEDObject* pLEDObject);

void turnOnLED(LEDObject* pLEDObject);
void turnOffLED(LEDObject* pLEDObject);

#endif // LED_H