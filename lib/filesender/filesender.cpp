/**
 * @file filesender.cpp
 * @author Adrian Saldana
 * @brief File Sender implementations. Supported ESP32 MCU-based SoC.
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "filesender.h"
#include "sdkconfig.h"
#include <stdlib.h>

static const char *TAG = "filesender";

FileSender::FileSender(fs::FS &fs, const char *path) : _fs(fs), _path(path) {}

bool FileSender::begin()
{
    if (!_fs.exists(_path))
    {
        ESP_LOGE(TAG, "File %s is unavailable in filesystem", _path);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

char *FileSender::getBuff()
{
    File file = _fs.open(_path);
    if (!file)
        return nullptr;
    return _buff;
}
