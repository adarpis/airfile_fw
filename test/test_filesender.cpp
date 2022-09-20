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

const char *FILE_PATH_TO_TEST_AVAILABLE = "500KB.out";
const char *FILE_PATH_TO_TEST_UNAVAILABLE = "unavailable.txt";
char path_variable[10];

void setUp(void)
{
    // set stuff up here
    strcpy(path_variable, FILE_PATH_TO_TEST_AVAILABLE);
}

void tearDown(void)
{
    // clean stuff up here
}

void test_initFileSender(void)
{
    TEST_ASSERT_MESSAGE(initFileSender(FILE_PATH_TO_TEST_AVAILABLE), "File available in SPDIFF partition");
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