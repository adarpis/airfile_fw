/**
 * @file test_filesender.cpp
 * @author Adrian Saldana
 * @brief
 * @version 0.1
 * @date 2022-09-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>
#include <unity.h>
#include <SPIFFS.h>
#include "filesender.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 8
#endif

#ifndef CONFIG_FILESENDER_SIZE_READ_BUFFER
#define CONFIG_FILESENDER_SIZE_READ_BUFFER 1024u
#endif

#define POSITION_MAX 500 * CONFIG_FILESENDER_SIZE_READ_BUFFER

const char *FILE_PATH_TO_TEST_AVAILABLE = "/500KB.out";
const char *FILE_PATH_TO_TEST_UNAVAILABLE = "/spiffs/unavailable.txt";
char path_variable[10];
uint8_t buffer[CONFIG_FILESENDER_SIZE_READ_BUFFER];

void setUp(void)
{
    // set stuff up here
    strcpy(path_variable, FILE_PATH_TO_TEST_AVAILABLE);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    SPIFFS.begin();
}

void tearDown(void)
{
    // clean stuff up here
    digitalWrite(LED_BUILTIN, LOW);
}

void test_initFileSender(void)
{
    FileSender fileSenderAvailable(SPIFFS, FILE_PATH_TO_TEST_AVAILABLE);
    FileSender fileSenderUnavailable(SPIFFS, FILE_PATH_TO_TEST_UNAVAILABLE);
    FileSender fileSenderAvailablePathV(SPIFFS, path_variable);
    TEST_ASSERT_FALSE_MESSAGE(fileSenderAvailable.begin(), "Unexpected file unavailable in SPDIFF partition");
    TEST_ASSERT_TRUE_MESSAGE(fileSenderUnavailable.begin(), "Unexpended file available in SPDIFF partition");
    TEST_ASSERT_FALSE_MESSAGE(fileSenderAvailablePathV.begin(), "Unexpected file unavailable in SPDIFF partition, path variable");
}

void test_getBuff(void)
{
    FileSender fileSender(SPIFFS, FILE_PATH_TO_TEST_AVAILABLE);
    if (!fileSender.begin())
    {
        size_t position = 0;
        do
        {
            size_t position_f = fileSender.getBuff(BUFFER_SZ(buffer));
            position += sizeof(buffer);
            TEST_ASSERT_EQUAL_INT(position, position_f);
        } while (position < POSITION_MAX);
    }
    else
        TEST_FAIL_MESSAGE("Fail into begin() method");
}

void setup()
{
    delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_initFileSender);
    RUN_TEST(test_getBuff);

    UNITY_END(); // stop unit testing
}

void loop()
{
}