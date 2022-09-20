/**
 * @file wirelessif.hpp
 * @author Adrian Saldana
 * @brief 
 * @version 0.1
 * @date 2022-09-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _WIRELESSIF_HPP
#define _WIRELESSIF_HPP

#include <Arduino.h>

void init_iot_client(void);
int publish(const char* topic, const char* payload = nullptr, size_t length = 0);
int disconnect();

#endif // _WIRELESSIF_HPP