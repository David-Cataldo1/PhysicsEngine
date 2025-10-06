#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


inline std::string ReadFile(std::string loc)
{
    std::string content;
    std::ifstream fileStream(loc, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cout << "Failed to read %s! File doesn't exist at " << loc << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

inline std::vector<std::string> ReadFileLineByLine(std::string loc)
{
    std::vector<std::string> content;
    std::ifstream fileStream(loc, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cout << "Failed to read %s! File doesn't exist at " << loc << std::endl;
        return std::vector<std::string>();
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.push_back(line);
    }

    fileStream.close();
    return content;
}
