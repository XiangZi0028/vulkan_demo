#pragma once
#include <iostream>
#include <fstream>
#include <vector>
namespace FileSys
{
    static std::vector<char> ReadFile(const std::string FilePath)
    {
        std::ifstream file(FilePath, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }
        size_t FileSize = (size_t)file.tellg();
        std::vector<char> buffer(FileSize);
        file.seekg(0);
        file.read(buffer.data(), FileSize);
        file.close();
        return buffer;
    }
}
