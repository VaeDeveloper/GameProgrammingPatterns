#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>


class FileSystem
{
public:
    virtual ~FileSystem() = default;
    virtual std::string Read(const std::string& path) = 0;
    virtual void Write(const std::string& path, const std::string& text) = 0;

    static FileSystem& Instance();

protected:
    FileSystem() = default;
};

#endif 
