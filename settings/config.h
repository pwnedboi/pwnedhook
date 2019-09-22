/*
 *  config.h
 */
#pragma once

using namespace std;

class config_manager_t
{
private:
    
    std::vector<std::string> m_configs;
    
public:
    
    void init();
    void load();
    void save();
    
    vector<std::string> get_configs() { return m_configs; }
};

extern config_manager_t g_config;

