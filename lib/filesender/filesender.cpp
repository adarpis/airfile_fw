/**
 * @file filesender.cpp
 * @author Adrian Saldana
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <SPIFFS.h>
#include <Arduino.h>
#include <stdlib.h>

bool initFileSender(const char* path)
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return EXIT_FAILURE;
    }
    
    if (!SPIFFS.exists(path))
    {
        Serial.println("Fatal error file doesn't exist, unavailable to send");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}