#include <iostream>
#include "FileSystem.h"

int main()
{
    try
    {
        FileSystem& fs = FileSystem::Instance();
        std::string filePath = "test.txt";
        std::string content = "Hello, cross-platform file system!";

        fs.Write(filePath, content);
        std::cout << "File written successfully.\n";

        std::string readContent = fs.Read(filePath);
        std::cout << "File content: " << readContent << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}