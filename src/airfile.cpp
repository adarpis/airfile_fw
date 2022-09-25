/**
 * @file airfile.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-09-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "sdkconfig.h"
#include <Arduino.h>
#include <USB.h>
#include <SPIFFS.h>
#include "wirelessif.h"
#include "fileslicer.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO (gpio_num_t) CONFIG_BLINK_GPIO

#ifndef LED_BUILTIN
#define LED_BUILTIN 8
#endif

#ifndef CONFIG_FILESLICER_SIZE_READ_BUFFER
#define CONFIG_FILESLICER_SIZE_READ_BUFFER 1024u
#endif

static const char *TAG = "airfile";

void blink_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while (1)
    {
        /* Blink off (output low) */
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 30       /* Time ESP32 will go to sleep (in seconds) */

void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
}

static void powerDown()
{
    delay(2000);
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    esp_deep_sleep_start();
}

#if !CONFIG_AUTOSTART_ARDUINO
#error "Configure CONFIG_AUTOSTART_ARDUINO=1 is required to compile, check configuration in sdkconfig.defaults"
#else
void setup()
{
    // Serial.begin(115200);
    // while (!Serial)
    // {
    //     ; // wait for serial port to connect. Needed for native USB
    // }
    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    pinMode(LED_BUILTIN, OUTPUT);

    print_wakeup_reason();                                            // Print the wakeup reason for ESP32
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    // ESP32 wakes up every 10 minutes
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF); // all RTC Peripherals are powered off

    if (!SPIFFS.begin(true))
    {
        ESP_LOGE(TAG, "An Error has occurred while mounting SPIFFS");
    }

    init_iot_client(); // Initialize Wifi and MQTT client, static IP and connect to broker

    delay(5000);
}

const char *FILE_PATH = "/500KB.out";

void loop()
{
    uint8_t buffer[CONFIG_FILESLICER_SIZE_READ_BUFFER];
    FileSlicer fileSlicer(SPIFFS, FILE_PATH);

    if (!fileSlicer.begin())
    {
        while (fileSlicer.getBuff(BUFFER_SZ(buffer)))
        {
            publish("file", (char *)buffer, sizeof(buffer));
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            delay(250);
        }
    }
    else
        ;

    delay(30000);
    ESP_LOGE(TAG, "Going to deep-sleep now");
    disconnect();
    esp_deep_sleep_start();
}
#endif
