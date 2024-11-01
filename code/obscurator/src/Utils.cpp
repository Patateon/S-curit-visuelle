#include <iostream>
#include <fstream>
#include <algorithm>
#include <Utils.cpp>

std::string getFileExtension(const std::string &fileName)
{
    std::string result;

    auto i = fileName.rbegin();
    while (i != fileName.rend())
    {
        if (*i == '.')
            break;

        result = *i + result;

        i++;
    }

    return result;
}

const char* getFileExtensionC(const char* fileName)
{   
    int i = 0;
    while(fileName[i] != '\0') i++;

    while(i > 0 && fileName[i] != '.')
        i--;
    
    return fileName + i + (i ? 1 : 0);
}

std::string getFileNameFromPath(const char *path)
{
    std::string name;
    int i = 0;

    for(; path[i] != '\0'; i++);

    for(; i >= 0 && path[i] != '/' && path[i] != '\\'; i--)
        name.push_back(path[i]);
    
    std::reverse(name.begin(), name.end());

    return name;
}

std::string getNameOnlyFromPath(const char *path)
{
    std::string name = getFileNameFromPath(path);
    std::string res;

    int i = 0;
    for(; name[i] != '\0' && name[i] != '.' ; i++)
        res += name[i];

    return res;
}