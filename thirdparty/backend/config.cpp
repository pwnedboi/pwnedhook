/*  config.cpp
 *
 *
 */
#include "config.h"
#include <unistd.h>
#include <pwd.h>

template<class T>
inline void debug(T msg, std::string prefix = "debug")
{
    std::cout << "[" + prefix + "] " << msg << std::endl;
}

config_manager_t::config_manager_t()
{
    passwd* pw = getpwuid(getuid());
    m_folder = std::string(pw->pw_dir) + "/";
    m_extention = ".txt";   // .pwn
    
    debug(m_folder);
}

static void set_value(json cfg, )
{
    
}

static void get_value()
{
    
}

static void set_color()
{
    
}

static void get_color()
{
    
}
