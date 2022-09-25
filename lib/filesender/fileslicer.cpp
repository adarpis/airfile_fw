/**
 * @file fileslicer.cpp
 * @author Adrian Saldana
 * @brief File Slicer implementations. Supported ESP32 MCU-based SoC.
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "fileslicer.h"
#include <stdlib.h>
static const char *TAG = "fileslicer";

FileSlicer::FileSlicer(fs::FS &fs, const char *path) : _fs(fs), _path(path) {}

FileSlicer::~FileSlicer()
{
    if (!_file)
        _file.close();
}

bool FileSlicer::begin()
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

size_t FileSlicer::getBuff(uint8_t *buff, size_t size)
{
    if (_file.available())
    {
        if (_file.read(buff, size))
        {
            size_t fPosition = _file.position();
            ESP_LOGI(TAG, "File position is %d", fPosition);
            return fPosition;
        }
    }
    return 0;
}
