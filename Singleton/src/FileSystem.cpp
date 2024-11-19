#include "FileSystem.h"
#include <fstream>
#include <stdexcept>


#ifdef _WIN32
#include <windows.h>

class WindowsFileSystem : public FileSystem
{
public:
    std::string Read(const std::string& path) override
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file: " + path);

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    }

    void Write(const std::string& path, const std::string& text) override
    {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file: " + path);

        file << text;
    }
};

#endif // _WIN32


#ifdef __linux__
#include <fstream>
#include <stdexcept>

class LinuxFileSystem : public FileSystem
{
public:
    std::string Read(const std::string& path) override
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file: " + path);

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    }

    void Write(const std::string& path, const std::string& text) override
    {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file: " + path);

        file << text;
    }
};

#endif // __linux__


FileSystem& FileSystem::Instance()
{
#ifdef _WIN32
    static WindowsFileSystem instance;
    return instance;
#elif __linux__
    static LinuxFileSystem instance;
    return instance;
#else
    throw std::runtime_error("Unsupported platform");
#endif
}


