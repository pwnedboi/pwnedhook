/*
 *  interface.h
 */
#pragma once

// todo : should be namespace::interface or something similar

typedef void* (*instantiate_interface_func)();

struct interface_reg_t
{
    instantiate_interface_func  m_create_fn;
    const char*                 m_name;
    interface_reg_t*            m_next;
};

inline void**& getvtable(void* inst, size_t offset = 0)
{
    return *reinterpret_cast<void***>((size_t)inst + offset);
}

inline const void** getvtable(const void* inst, size_t offset = 0)
{
    return *reinterpret_cast<const void***>((size_t)inst + offset);
}

template<typename fn>
inline fn getvfunc(void* inst, size_t index, size_t offset = 0)
{
    return reinterpret_cast<fn>(getvtable(inst, offset)[index]);
}

template <typename interface>
interface* create_interface(const char* filename, const char* version, bool exact = false)
{
    void* library = dlopen(filename, RTLD_NOLOAD | RTLD_NOW);
    
    if (!library)
        return nullptr;
    
    void* sym = dlsym(library, "s_pInterfaceRegs");
    
    if (!sym)
    {
        dlclose(library);
        return nullptr;
    }
    
    dlclose(library);
    
    interface_reg_t* interfaces = *((interface_reg_t**)sym);
    interface_reg_t* cur_interface;
    
    for (cur_interface = interfaces; cur_interface; cur_interface = cur_interface->m_next)
    {
        const char* name = cur_interface->m_name;
        
        // error
        if(!name)
            // todo : print error
            continue;
        
        if (exact)
        {
            if (strcmp(name, version) == 0)
                return (interface*)cur_interface->m_create_fn();
        }
        else
        {
            if (strstr(name, version) && strlen(name) - 3 == strlen(version))
                return (interface*)cur_interface->m_create_fn();
        }
    }
    
    return nullptr;
}

inline void dump_interfaces()
{
    // todo : add more
    vector<string> modules = {"./csgo/bin/osx64/client_panorama.dylib", "./bin/osx64/engine.dylib", };
    
    std::ofstream f;
    f.open("netvar_dump.txt");
    
    for (auto module : modules)
    {
        void* library = dlopen(module.c_str(), RTLD_NOLOAD | RTLD_NOW);
        
        if (!library)
            continue;
        
        void* interfaces_sym = dlsym(library, "s_pInterfaceRegs");
        
        if (!interfaces_sym)
        {
            dlclose(library);
            continue;
        }
        
        dlclose(library);
        
        interface_reg_t* interfaces = *(interface_reg_t**)(interfaces_sym);
        interface_reg_t* cur_interface;
        
        vector<string> names;
        
        for (cur_interface = interfaces; cur_interface; cur_interface = cur_interface->m_next)
        {
            if(!cur_interface->m_name)
                continue;
            
            names.push_back(string(cur_interface->m_name));
        }
        
        if (names.empty())
            continue;
        
        f << std::string(module) << std::endl;
        
        for (auto interface : names)
            f << "\t" << interface << std::endl;
        
        f << std::endl;
    }
    
    f.close();
}
