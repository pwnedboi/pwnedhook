/*      backtrack.cpp
 *
 *  https://www.unknowncheats.me/forum/counterstrike-global-offensive/352759-properly-backtracking-entity.html
 *
 */
#include "common.h"
#include "renderer.h"
#include "backtrack.h"

backtrack_t* g_backtrack = new backtrack_t();

struct backtrack_info_t
{
    int     tick_count;
    float   lby;
    float   sim_time;
    vec3_t  head_pos;
    vec3_t  position;
};

static array<array<backtrack_info_t, 12>, 64> info;

void backtrack_t::store()
{
    if(!set.legit.backtrack && !set.rage.backtrack)
        return;
    
    if(!global::cmd || !global::local)
        return;
    
    for(int i = 0; i < g_globals->m_max_clients; i++)
    {
        player_t* player = g_ent_list->get_player(i);
        
        if(!player)
            continue;
        
        if(player == global::local || player->get_team() == global::local->get_team())
            continue;
        
        if(player->is_dormant() || player->is_immune())
            continue;
        
        if(!player->is_alive())
        {
            // they died so reset their records
            memset(info.at(i).data(), 0, sizeof(backtrack_info_t) * 12);
            continue;
        }
        
        backtrack_info_t new_record;
        new_record.tick_count   = g_globals->m_tick_count;
        new_record.lby          = player->get_lower_body_yaw();
        new_record.sim_time     = player->get_simulation_time();
        new_record.head_pos     = get_hitbox_position(player, 1);
        new_record.position     = player->get_origin();
        
        info.at(i).at(global::cmd->m_command_number % 12) = new_record;
    }
}

inline vec3_t angle_vectors(qangle_t ang)
{
    auto sy = sin(ang.y / 180.f * static_cast<float>(M_PI));
    auto cy = cos(ang.y / 180.f * static_cast<float>(M_PI));
    
    auto sp = sin(ang.x / 180.f * static_cast<float>(M_PI));
    auto cp = cos(ang.x / 180.f * static_cast<float>(M_PI));
    
    return vec3_t(cp * cy, cp * sy, -sp);
}

inline float distance_to_point(vec3_t point, vec3_t origin, vec3_t dir)
{
    vec3_t point_dir = point - origin;
    
    float offset = point_dir.dot(dir) / (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    
    if (offset < 0.000001f)
        return MAXFLOAT;
    
    vec3_t perp_point = origin + (dir * offset);
    
    return (point - perp_point).length();
}

static player_t* find_target()
{
    player_t* target = nullptr;
    
    float best_fov  = MAXFLOAT;
    
    for(int i = 0; i < g_globals->m_max_clients; i++)
    {
        player_t* player = g_ent_list->get_player(i);
        
        if(!player)
            continue;
        
        if(player == global::local)
            continue;
        
        if(player->get_team() == global::local->get_team())
            continue;
        
        if(!player->is_alive() || !player->is_player() || player->is_dormant() || player->is_immune())
            continue;
        
        vec3_t   hitbox_pos = get_hitbox_position(player, 1) + (player->get_velocity() * g_globals->m_interval_per_tick);
        qangle_t aim_ang    = calculate_angle(global::local->get_eye_position(), hitbox_pos);
        float    fov        = get_fov(global::cmd->m_view_angles, aim_ang);
        
        if(fov < best_fov)
        {
            target   = player;
            best_fov = fov;
        }
    }
    
    return target;
}

void backtrack_t::backtrack_player(player_t* player)
{
    if(!set.legit.backtrack && !set.rage.backtrack)
        return;
    
    if(!player)
        player = find_target();
    
    if(!player)
        return;
    
    if(!player->is_alive())
        return;
    
    int best_tick       = -1;
    int index           = player->get_index();
    int update_tick     = -1;
    float best_dist     = MAXFLOAT;
    vec3_t view_dir     = angle_vectors(global::cmd->m_view_angles + (*global::local->get_aim_punch_angle() * 2));
    backtrack_info_t best_record;
    
    for(int i = 0; i < 12; i++)
    {
        auto record = info.at(index).at(i);
        float dist  = distance_to_point(record.head_pos, global::local->get_eye_position(), view_dir);
        
        if(i > 0 && info.at(index).at(i - 1).lby != record.lby)
            update_tick = i;
        
        if(dist < best_dist)
        {
            best_record = record;
            best_tick   = i;
        }
    }
    
    // only need to add the lerp if we disabling interp
    float lerp_time = 0.f;
    
    if(global::cmd->m_buttons & IN_ATTACK)
        global::cmd->m_tick_count = TIME_TO_TICKS(best_record.sim_time + lerp_time);
}

void backtrack_t::draw(player_t* player)
{
    int index = player->get_index();
    
    for(int i = -1; i < 12 - 1; i++)
    {
        auto record = info.at(index).at(i);
        
        vec3_t screen_pos;
        
        if(!world_to_screen(record.head_pos, screen_pos))
            continue;
        
        g_render->draw_box_filled(screen_pos.x, screen_pos.y, 2, 2, color_t::blue);
    }
}
