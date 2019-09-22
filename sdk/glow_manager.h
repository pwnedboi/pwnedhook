/*
 *  glow_manager.h
 */
#pragma once

#define END_OF_FREE_LIST    -1
#define ENTRY_IN_USE        -2

struct glow_object_t
{
    base_entity_t*  m_entity;
    vec3_t          m_flow_color;
    float           m_glow_alpha;
    char            m_unknown0[4];
    float           m_unk0;
    float           m_bloom_amount;
    float           m_local_player_is_zero_point3;
    bool            m_render_when_occluded;
    bool            m_render_when_unoccluded;
    bool            m_full_bloom_render;
    char            m_unknown1[1];
    int             m_full_bloom_stencil_test_value;
    int             m_unk1;
    int             m_split_screen_slot;
    int             m_next_free_slot;
    
    bool is_unused() const
    {
        return m_next_free_slot != ENTRY_IN_USE;
    }
};

class glow_manager_t
{
public:
    
    int register_glow_object(base_entity_t* entity)
    {
        if (m_first_free_slot == END_OF_FREE_LIST)
            return -1;
        
        int index = m_first_free_slot;
        m_first_free_slot = m_entries[index].m_next_free_slot;
        
        m_entries[index].m_entity = entity;
        m_entries[index].m_unk0 = 0.0f;
        m_entries[index].m_local_player_is_zero_point3 = 0.0f;
        m_entries[index].m_full_bloom_render = false;
        m_entries[index].m_full_bloom_stencil_test_value = 0;
        m_entries[index].m_split_screen_slot = -1;
        m_entries[index].m_next_free_slot = ENTRY_IN_USE;
        
        return index;
    }
    
    void unregister_glow_object(int index)
    {
        m_entries[index].m_next_free_slot = m_first_free_slot;
        m_entries[index].m_entity = NULL;
        m_first_free_slot = index;
    }
    
    bool has_glow_effect(base_entity_t* entity)
    {
        for (int i = 0; i < m_entries.count(); ++i)
        {
            if (m_entries[i].m_entity != entity)
                continue;
            
            if (!m_entries[i].is_unused())
                return true;
        }
        
        return false;
    }
    
public:
    
    CUtlVector<glow_object_t> m_entries;
    int m_first_free_slot;
    
};
