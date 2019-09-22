/*
 * vmt.h
 */
#pragma once

class vmt_t
{
private:
    
    uintptr_t* vmt;
    
public:
    
    uintptr_t** interface      = nullptr;
    uintptr_t*  original_vmt   = nullptr;
    size_t      count          = 0;    // number of methods
    
public:
    
    vmt_t(void* interface)
    {
        this->interface = reinterpret_cast<uintptr_t**>(interface);
        
        while (reinterpret_cast<uintptr_t*>(*this->interface)[count])
            count++;
        
        original_vmt = *this->interface;
        
        vmt = new uintptr_t[sizeof(uintptr_t) * count];
        
        memcpy(vmt, original_vmt, sizeof(uintptr_t) * count);
    }
    
    // hook virtual method
    void hook(void* method, size_t index)
    {
        vmt[index] = reinterpret_cast<uintptr_t>(method);
    }
    
    template<typename fn>
    fn get_original_method(size_t index)
    {
        return reinterpret_cast<fn>(original_vmt[index]);
    }
    
    void apply()
    {
        *this->interface = vmt;
    }
    
    void release()
    {
        // restore it to how it was before
        *this->interface = original_vmt;
    }
};
