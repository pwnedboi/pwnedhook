/*
 *  base_planted_c4.h
 */
#pragma once

class base_planted_c4_t : public base_entity_t
{
public:
    
    bool is_ticking()
    {
        return *(bool*)((uintptr_t)this + g_offsets.planted_c4.m_bomb_ticking);
    }
    
    float get_blow_time()
    {
        return *(float*)((uintptr_t)this + g_offsets.planted_c4.m_blow_time);
    }
    
    bool is_defused()
    {
        return *(bool*)((uintptr_t)this + g_offsets.planted_c4.m_bomb_defused);
    }
    
    int get_defuser()
    {
        return *(int*)((uintptr_t)this + g_offsets.planted_c4.m_bomb_defuser);
    }
    
    float get_defuse_countdown()
    {
        return *(float*)((uintptr_t)this + g_offsets.planted_c4.m_defuse_count_down);
    }    
};

typedef base_planted_c4_t planted_c4_t;

class base_c4_t : public base_entity_t
{
public:
    
    // todo
    // offsets.DT_WeaponC4.m_bStartedArming
    
};
