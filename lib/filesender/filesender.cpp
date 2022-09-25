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

FileSender::~FileSender()
{
    if (!_file)
        _file.close();
}

bool FileSender::begin()
{
    if (!_fs.exists(_path))
    {
        ESP_LOGE(TAG, "File %s is unavailable in filesystem", _path);
        return EXIT_FAILURE;
    }

    _file = _fs.open(_path);
    if (!_file)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

uint8_t *FileSender::getBuff()
{
    if (_file.available())
    {
        if (_file.read(_buff, (size_t)CONFIG_FILESENDER_SIZE_READ_BUFFER))
            return _buff;
    }
    return nullptr;
}
