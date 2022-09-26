/**
 * @file fileslicer.hpp
 * @author Adrian Saldana
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _FILE_SLICER_HPP
#define _FILE_SLICER_HPP

#include <FS.h>

/**
 * @brief
 *
 */
class FileSlicer
{
public:
    FileSlicer(fs::FS &fs, const char *path);
    ~FileSlicer();

    bool begin();
    size_t getBuff(uint8_t *buff, size_t size);

private:
protected:
    FS _fs;
    File _file;
    const char *_path;
};

#endif //_FILE_SLICER_HPP
