/*
 *  cvar.h
 */
#pragma once

class convar_t
{
public:
    
    float get_float()
    {
        typedef float (*o_get_float)(void*);
        return getvfunc<o_get_float>(this, 15)(this);
    }
    
    int get_int()
    {
        typedef int (*o_get_int)(void*);
        return getvfunc<o_get_int>(this, 16)(this);
    }
    
    void set_value(const char* value)
    {
        typedef void (*o_set_value)(void*, const char*);
        return getvfunc<o_set_value>(this, 17)(this, value);
    }
    
    void set_value(float value)
    {
        typedef void (*o_set_value)(void*, float);
        return getvfunc<o_set_value>(this, 18)(this, value);
    }
    
    void set_value(int value)
    {
        typedef void (*o_set_value)(void*, int);
        return getvfunc<o_set_value>(this, 19)(this, value);
    }
    
    void set_value(color_t value)
    {
        typedef void (*o_set_value)(void*, color_t);
        return getvfunc<o_set_value>(this, 20)(this, value);
    }
    
    char    pad_0x0000[0x4];
    convar_t* m_next;
    int32_t m_registered;
    char*   m_name;
    char*   m_help_string;
    int32_t m_flags;
    char    pad_0x0018[0x4];
    convar_t* m_parent;
    char*   m_default_value;
    char*   m_string;
    int32_t m_string_length;
    float   m_value_float;
    int32_t m_value_int;
    int32_t m_has_min;
    float   m_min_val;
    int32_t m_has_max;
    float   m_max_val;
    void*   m_change_callback_fn;
    
};

class cvar_t
{
public:
    
    convar_t* find_var(const char* var)
    {
        typedef convar_t* (*o_find_var)(void*, const char*);
        return getvfunc<o_find_var>(this, 15)(this, var);
    }
    
    template <typename... values>
    void console_print_color(const color_t& color, const char* message, values... params)
    {
        typedef void (*o_console_print_color)(void*, const color_t&, const char*, ...);
        return getvfunc<o_console_print_color>(this, 25)(this, color, message, params...);
    }
    
    template <typename... values>
    void console_print(const char* message, values... params)
    {
        typedef void (*o_console_print)(void*, const char*, ...);
        return getvfunc<o_console_print>(this, 27)(this, message, params...);
    }
    
};

#define FORCEINLINE_CVAR

#pragma region CVAR Flags

#define FCVAR_NONE                      0
#define FCVAR_UNREGISTERED              (1<<0)
#define FCVAR_DEVELOPMENTONLY           (1<<1)
#define FCVAR_GAMEDLL                   (1<<2)
#define FCVAR_CLIENTDLL                 (1<<3)
#define FCVAR_HIDDEN                    (1<<4)
#define FCVAR_PROTECTED                 (1<<5)
#define FCVAR_SPONLY                    (1<<6)
#define FCVAR_ARCHIVE                   (1<<7)
#define FCVAR_NOTIFY                    (1<<8)
#define FCVAR_USERINFO                  (1<<9)
#define FCVAR_CHEAT                     (1<<14)
#define FCVAR_PRINTABLEONLY             (1<<10)
#define FCVAR_UNLOGGED                  (1<<11)
#define FCVAR_NEVER_AS_STRING           (1<<12)
#define FCVAR_REPLICATED                (1<<13)
#define FCVAR_DEMO                      (1<<16)
#define FCVAR_DONTRECORD                (1<<17)
#define FCVAR_NOT_CONNECTED             (1<<22)
#define FCVAR_ARCHIVE_XBOX              (1<<24)
#define FCVAR_SERVER_CAN_EXECUTE        (1<<28)
#define FCVAR_SERVER_CANNOT_QUERY       (1<<29)
#define FCVAR_CLIENTCMD_CAN_EXECUTE     (1<<30)

#pragma endregion
