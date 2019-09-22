/*
 * global_vars.h
 */
#pragma once

class global_vars_t
{
public:
    
    global_vars_t(bool is_client);
    bool is_client() const;
    int  get_network_base(int tick, int entity);
    
public:
    
    float       m_real_time;                        // 0x00
    int         m_frame_count;                      // 0x4
    float       m_absolute_frame_time;              // 0x8
    float       m_absolute_frame_start_time_stddev; // 0xC
    float       m_cur_time;                         // 0x10
    float       m_frame_time;
    int         m_max_clients;
    int         m_tick_count;
    float       m_interval_per_tick;
    float       m_interpolation_amount;
    int         m_sim_ticks_this_frame;
    int         m_network_protocol;
    void*       m_save_date;
    
private:
    
    bool        m_is_client;
    
public:
    
    bool        m_is_remote_client;
    
private:
    
    int         m_timestamp_networking_base;
    int         m_timestamp_randomise_window;
    
};
