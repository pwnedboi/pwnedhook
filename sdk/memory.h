/*
 *  pat_scan.h
 */
#pragma once

struct memory_module_t
{
    off_t       length;
    uint64_t    address;
    char*       path;
    string      fullpath;
    unsigned char* buffer = nullptr;
};

class memory_manager_t
{
private:
    
    map<string, memory_module_t> loaded_modules;
    
private:
    
    bool compare(const unsigned char* data, const unsigned char* sig, const char* mask);
    
    // unused
    string base_name(string const& path_name);
    string get_module_path(string image_name);
    
public:
    
    memory_manager_t();
    
    uintptr_t find_pattern(uintptr_t addr, off_t len, unsigned char* sig, const char* mask, uintptr_t offset = 0x0);
    uintptr_t get_pointer(string name, unsigned char* sig, const char* mask, uint32_t start);
    uintptr_t get_procedure(string name, unsigned char* sig, const char* mask, uintptr_t offset = 0x0);
    uintptr_t get_base_address(string name);
    uintptr_t get_absolue_address(uintptr_t ptr, uintptr_t start_offset, uintptr_t size);
    
    void protect_addr(void* addr, int prot);
    
    void load_modules();
};

extern memory_manager_t* g_memory;
