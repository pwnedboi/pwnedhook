/*
 *  input_system.h
 */
#pragma once

class input_system_t
{
public:
    
    void enable_input(bool enable)
    {
        typedef void (*o_enable_input)(void*, bool);
        return getvfunc<o_enable_input>(this, 11)(this, enable);
    }
    
    bool is_button_down(button_code_t code)
    {
        typedef bool (*o_is_button_down)(void*, button_code_t);
        return getvfunc<o_is_button_down>(this, 15)(this, code);
    }
    
    void reset_input_state()
    {
        typedef void (*o_reset_input_state)(void*);
        return getvfunc<o_reset_input_state>(this, 39)(this);
    }
    
    const char* button_code_to_string(button_code_t code)
    {
        typedef const char* (*o_button_code_to_string)(void*, button_code_t);
        return getvfunc<o_button_code_to_string>(this, 40)(this, code);
    }
    
    button_code_t virtual_key_to_button_code(int virtual_key)
    {
        typedef button_code_t (*o_virtual_key_to_button_code)(void*, int);
        return getvfunc<o_virtual_key_to_button_code>(this, 44)(this, virtual_key);
    }
    
    int button_code_to_virtual_key(button_code_t code)
    {
        typedef int (*o_button_code_to_virtual_key)(void*, button_code_t);
        return getvfunc<o_button_code_to_virtual_key>(this, 45)(this, code);
    }
    
    void get_cursor_position(int* x, int* y)
    {
        typedef void (*o_get_cursor_position)(void*, int*, int*);
        return getvfunc<o_get_cursor_position>(this, 56)(this, x, y);
    }
};
