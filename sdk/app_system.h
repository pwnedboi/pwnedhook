/*
 *  app_system.h
 */
#pragma once

struct app_system_info_t
{
    const char* m_mod_name;
    const char* m_interface_name;
};

enum init_return_value_t
{
    INIT_FAILED = 0,
    INIT_OK,
    INIT_LAST_VALUE,
};

enum app_system_tier_t
{
    APP_SYSTEM_TIER0 = 0,
    APP_SYSTEM_TIER1,
    APP_SYSTEM_TIER2,
    APP_SYSTEM_TIER3,
    APP_SYSTEM_TIER_OTHER,
};

typedef void* (*create_interface_fn) (const char*, int*);

class app_system_t
{
public:
    
    // Here's where the app systems get to learn about each other
    virtual bool connect(create_interface_fn factory) = 0;
    
    virtual void disconnect() = 0;
    
    // Here's where systems can access other interfaces implemented by this object
    // Returns nullptr if it doesn't implement the requested interface
    virtual void* query_interface(const char* interface_name) = 0;
    
    // Init, shutdown
    virtual init_return_value_t init() = 0;
    
    virtual void shutdown() = 0;
    
    // Returns all dependent libraries
    virtual const app_system_info_t* get_dependencies() = 0;
    
    // Returns the tier
    virtual app_system_tier_t get_tier() = 0;
    
    // Reconnect to a particular interface
    virtual void reconnect(create_interface_fn factory, const char* interface_name) = 0;
    
    // Returns whether or not the app system is a singleton
    virtual bool is_singleton() = 0;
};
