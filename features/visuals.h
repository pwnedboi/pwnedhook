/*
 *  visuals.h
 */
#pragma once

class visuals_t
{
private:
    
    vector<float> m_player_alpha;
    
public:
    
    visuals_t()
    {
        m_player_alpha.resize(64);
    }
    
    void draw_player_esp();
    void draw_other_esp();
    
    void draw_hitmarkers();
    void draw_scope();
    
    void remove_flash();
    void remove_smoke();
};

extern visuals_t* g_visuals;
