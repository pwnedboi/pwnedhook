/*
 *  material_system.h
 */
#pragma once

typedef unsigned short material_handle_t;

class key_values_t;

class material_system_t : public app_system_t
{
public:
    
    material_t* create_material(const char* mat_name, key_values_t* key_values)
    {
        typedef material_t* (*o_create_material)(void*, const char*, key_values_t*);
        return getvfunc<o_create_material>(this, 83)(this, mat_name, key_values);
    }
    
    material_t* find_material(char const* mat_name, const char* tex_group_name, bool complain = true, const char* complain_prefix = NULL)
    {
        typedef material_t* (*o_find_material)(void*, char const*, const char*, bool, const char*);
        return getvfunc<o_find_material>(this, 84)(this, mat_name, tex_group_name, complain, complain_prefix);
    }
    
    material_handle_t first_material()
    {
        typedef material_handle_t (*o_first_material)(void*);
        return getvfunc<o_first_material>(this, 86)(this);
    }
    
    material_handle_t next_material(material_handle_t h)
    {
        typedef material_handle_t (*o_next_material)(void*, material_handle_t);
        return getvfunc<o_next_material>(this, 87)(this, h);
    }
    
    material_handle_t invalid_material()
    {
        typedef material_handle_t (*o_invalid_material)(void*);
        return getvfunc<o_invalid_material>(this, 88)(this);
    }
    
    material_t* get_material(material_handle_t h)
    {
        typedef material_t* (*o_get_material)(void*, material_handle_t);
        return getvfunc<o_get_material>(this, 89)(this, h);
    }
};
