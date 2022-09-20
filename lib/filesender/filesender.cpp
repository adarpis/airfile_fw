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

#include "SPIFFS.h"
#include <Arduino.h>
#include <stdlib.h>

bool initFileSender(const char* path)
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return EXIT_FAILURE;
    }
    File file = SPIFFS.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return EXIT_FAILURE;
    }
    file.close();
    return EXIT_SUCCESS;
}