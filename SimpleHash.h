#pragma once
#include <string>
struct SimpleHash 
{
    size_t operator()(const std::string& s) const 
    {
        size_t hash = 0;
        for (const char& c : s)
        {
            hash = (hash * 31) + c;
        }
        return hash;
    }
};