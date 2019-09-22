/*
 *  backtrack.h
 */
#pragma once

#define TIME_TO_TICKS(time)     ((int)(0.5f + ((float)time) / g_globals->m_interval_per_tick ))

class backtrack_t
{
private:
    
private:
    
public:
    
    void store();
    void backtrack_player(player_t* player);
    void draw(player_t* player);
    
};

extern backtrack_t* g_backtrack;
