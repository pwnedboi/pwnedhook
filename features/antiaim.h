/*
 *  antiaim.h
 */
#pragma once

// pretty much just rage movement
class antiaim_manager_t
{
private:
    
    bool m_broke_last = false;
    
private:

    
public:
    
    void anti_aim();
    
    
};

extern antiaim_manager_t* g_antiaim;
