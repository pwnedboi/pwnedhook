/*
 * config.h
 */
#pragma once

#include <string>
#include <vector>
#include <iostream>

using namespace std;

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class config_manager_t
{
private:
    
    std::string m_folder;
    std::string m_extention;
    
    std::vector<std::string> m_configs;
    
private:
    
    std::vector<std::string> find_configs();
    
public:
    
    config_manager_t();
    
    
    
    inline std::vector<std::string> get_configs() { return m_configs; }
};
