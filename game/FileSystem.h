#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "utils/bytearray.h"
#include <string>

class FileSystem {
public:
    std::string m_resource_path;

    static FileSystem& instance()
    {
        static FileSystem singleton;
        return singleton;
    }

    ~FileSystem();

    bool load_from_file(const std::string& path, bytearray& data);
    bool save_to_file(const std::string& path, bytearray& data);

    FileSystem(const FileSystem& root) = delete;
    FileSystem& operator=(const FileSystem&) = delete;

private:
    FileSystem();
};

#endif //FILESYSTEM_H
