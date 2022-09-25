/**
 * @file filesender.hpp
 * @author Adrian Saldana
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _FILE_SENDER_HPP
#define _FILE_SENDER_HPP

#include <FS.h>

/**
 * @brief
 *
 */
class FileSender
{
public:
    FileSender(fs::FS &fs, const char *path);
    ~FileSender();

    bool begin();
    size_t getBuff(uint8_t *buff, size_t size);

private:
protected:
    FS _fs;
    File _file;
    const char *_path;
};

#endif //_FILE_SENDER_HPP
