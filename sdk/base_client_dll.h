/*
 * base_client_dll.h
 */
#pragma once

class base_client_dll_t
{
public:
    
    client_class_t* get_all_classes()
    {
        typedef client_class_t* (*o_get_all_classes)(void*);
        return getvfunc<o_get_all_classes>(this, 8)(this);
    }    
};
