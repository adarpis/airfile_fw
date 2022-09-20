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
#include "filesender.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 8
#endif

const char *FILE_PATH_TO_TEST_AVAILABLE = "/500KB.out";
const char *FILE_PATH_TO_TEST_UNAVAILABLE = "/spiffs/unavailable.txt";
char path_variable[10];

void setUp(void)
{
    // set stuff up here
    strcpy(path_variable, FILE_PATH_TO_TEST_AVAILABLE);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void tearDown(void)
{
    // clean stuff up here
    digitalWrite(LED_BUILTIN, LOW);
}

void test_initFileSender(void)
{
    TEST_ASSERT_FALSE_MESSAGE(initFileSender(FILE_PATH_TO_TEST_AVAILABLE), "Unexpected file unavailable in SPDIFF partition");
    TEST_ASSERT_TRUE_MESSAGE(initFileSender(FILE_PATH_TO_TEST_UNAVAILABLE), "Unexpended file available in SPDIFF partition");
    TEST_ASSERT_FALSE_MESSAGE(initFileSender(path_variable), "Unexpected file unavailable in SPDIFF partition, path from RAM");
}

void setup()
{
    delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_initFileSender);

    UNITY_END(); // stop unit testing
}

void loop()
{
}