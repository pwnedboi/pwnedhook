/*
 *  legitbot.h
 */
#pragma once

class legitbot_t
{
private:
    
    player_t*   m_target;
    int         m_target_index;
    int         m_target_hitbox;
    
private:
    
    void    find_closest_target();
    int     find_closest_hitbox(player_t* player);
    vec3_t  get_aim_position(player_t* player, int hitbox);
    bool    can_hit_target();
    
public:
    
    void aimbot();
    
};

extern legitbot_t* g_legit;
