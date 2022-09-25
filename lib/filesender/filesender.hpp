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

#ifndef CONFIG_FILESENDER_SIZE_READ_BUFFER
#define CONFIG_FILESENDER_SIZE_READ_BUFFER 1024
#endif

#include <FS.h>

/**
 * @brief
 *
 */
class FileSender
{
public:
    FileSender(fs::FS& fs, const char* path);
    ~FileSender();

    bool begin();
    char *getBuff();

private:
    char _buff[CONFIG_FILESENDER_SIZE_READ_BUFFER];

protected:
    FS _fs;
    const char *_path;
};

#endif //_FILE_SENDER_HPP
