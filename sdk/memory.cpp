/*      pat_scan.cpp
 *
 *
 *
 */

#include "common.h"
#include "memory.h"

#include <sys/mman.h>
#include <mach-o/dyld_images.h>

memory_manager_t* g_memory = nullptr;

memory_manager_t::memory_manager_t()
{
    
}

std::string memory_manager_t::base_name(const std::string& pathname)
{
    return std::string(find_if(pathname.rbegin(), pathname.rend(), [](char ch) { return ch == '\\' || ch == '/'; }).base(), pathname.end());
}

void memory_manager_t::load_modules()
{
    struct task_dyld_info dyld_info;
    vm_address_t address = 0;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    
    if(task_info(current_task(), TASK_DYLD_INFO, (task_info_t)&dyld_info, &count) == KERN_SUCCESS)
        address = (vm_address_t)dyld_info.all_image_info_addr;
    
    struct dyld_all_image_infos* dyldaii;
    mach_msg_type_number_t size = sizeof(dyld_all_image_infos);
    vm_offset_t readMem;
    kern_return_t kr = vm_read(current_task(), address, size, &readMem, &size);
    
    if(kr != KERN_SUCCESS)
        return;
    
    dyldaii = (dyld_all_image_infos*) readMem;
    
    int imageCount = dyldaii->infoArrayCount;
    mach_msg_type_number_t dataCnt  = imageCount * 24;
    struct dyld_image_info* g_dii   = (struct dyld_image_info*)malloc(dataCnt);
    
    // 32bit bs 64bit
    kern_return_t kr2 = vm_read(current_task(), (vm_address_t)dyldaii->infoArray, dataCnt, &readMem, &dataCnt);
    
    if (kr2)
    {
        free(g_dii);
        return;
    }
    
    struct dyld_image_info* dii = (struct dyld_image_info*)readMem;
    
    for (int i = 0; i < imageCount; i++)
    {
        dataCnt = 1024;
        vm_read(current_task(), (vm_address_t)dii[i].imageFilePath, dataCnt, &readMem, &dataCnt);
        
        char* imageName = (char*)readMem;
        
        if (imageName)
            g_dii[i].imageFilePath = strdup(imageName);
        else
            g_dii[i].imageFilePath = NULL;
        
        g_dii[i].imageLoadAddress = dii[i].imageLoadAddress;
        
        memory_module_t memoryModule;
        struct stat st;
        stat(imageName, &st);
        
        memoryModule.address    = (vm_address_t)dii[i].imageLoadAddress;
        memoryModule.length     = st.st_size;
        memoryModule.path       = imageName;
        memoryModule.fullpath   = g_dii[i].imageFilePath;
        
        std::string base = base_name(imageName);
        this->loaded_modules[base] = memoryModule;
    }
    
    free(g_dii);    
}

bool memory_manager_t::compare(const unsigned char* pData, const unsigned char* bMask, const char* szMask)
{
    for (; *szMask; ++szMask, ++pData, ++bMask)
    {
        if (*szMask == 'x' && *pData != *bMask)
            return false;
    }
    
    return (*szMask) == 0;
}

uintptr_t memory_manager_t::find_pattern(uintptr_t dwAddress, off_t dwLen, unsigned char* bMask, const char* szMask, uintptr_t offset)
{
    for (uintptr_t i = offset; i < dwLen; i++)
    {
        if (compare((unsigned char*)(dwAddress + i), bMask, szMask))
            return (uintptr_t)(dwAddress + i);
    }
    
    return 0;
}

uintptr_t memory_manager_t::get_pointer(string imageName, unsigned char* bMask, const char* szMask, uint32_t start)
{
    memory_module_t module = loaded_modules[imageName];
    uintptr_t dwAddress = module.address;
    off_t dwLen = module.length;
    
    uintptr_t signatureAddress = find_pattern(dwAddress, dwLen, bMask, szMask) + start;
    uintptr_t fileOffset = signatureAddress - dwAddress;
    uintptr_t offset = *reinterpret_cast<uint32_t*>(signatureAddress);
    
    return dwAddress + (offset + fileOffset);
}

uintptr_t memory_manager_t::get_procedure(string name, unsigned char* sig, const char* mask, uintptr_t offset)
{
    memory_module_t module = loaded_modules[name];
    uintptr_t dwAddress = module.address;
    off_t dwLen = module.length;
    
    return find_pattern(dwAddress, dwLen, sig, mask, offset);
}

string memory_manager_t::get_module_path(string imageName)
{
    return loaded_modules[imageName].path;
}

uintptr_t memory_manager_t::get_base_address(string imageName)
{
    memory_module_t module = loaded_modules[imageName];
    return module.address;
}

uintptr_t memory_manager_t::get_absolue_address(uintptr_t ptr, uintptr_t start_offset, uintptr_t size)
{
    return ptr + *reinterpret_cast<uint32_t*>(ptr + start_offset) + size;
}

void memory_manager_t::protect_addr(void* addr, int prot)
{
    long pagesize = sysconf(_SC_PAGESIZE);
    void* address = (void*)((long)(uintptr_t)addr & ~(pagesize - 1));
    mprotect(address, sizeof(address), prot);
}
