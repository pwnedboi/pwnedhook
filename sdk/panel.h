/*
 *  panel.h
 */
#pragma once

class panel_t
{
public:
    
    const char* get_name(VPANEL vguiPanel)
    {
        typedef const char*(*oget_name)(void*, VPANEL);
        return getvfunc<oget_name>(this, 37)(this, vguiPanel);
    }    
};
