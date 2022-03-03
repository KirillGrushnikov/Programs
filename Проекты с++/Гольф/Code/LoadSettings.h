#pragma once
#include "tinyxml/tinyxml.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

struct Param
{
    std::string name;
    float param;
};

class Settings
{
private:
    std::string FileName;
    std::vector<Param> params;
    bool LoadParam = false;
public:
    Settings() {}
    Settings(std::string filename)
    {
        FileName = filename;
        LoadParam = LoadFromFile(filename);
    }
    ~Settings() {}

    Param& GetParam(const std::string& name);
    std::vector<Param> GetParams(const std::string& name);
    std::vector<Param> GetAllParams();

    bool LoadFromFile(std::string filename);
    void Save();
};