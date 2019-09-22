/*  autowall.cpp
 *
 *
 */
#include "common.h"
#include "ragebot.h"

struct fire_bullet_data_t
{
    vec3_t          m_src;
    trace_t         m_trace;
    vec3_t          m_direction;
    trace_filter_t  m_filter;
    float           m_length;
    float           m_lenth_remaining;
    float           m_damage;
    int             m_count; // penetration_count
    player_t*       m_target;
};

/*
 *  get_damage_multiplier
 *  Returns the damage multiplier for the given hitgroup
 */
static float get_damage_multiplier(hit_group_t group)
{
    switch(group)
    {
        case HITGROUP_HEAD:
            return 4.f;
            
        case HITGROUP_STOMACH:
            return 1.25f;
            
        case HITGROUP_LEFTLEG:
        case HITGROUP_RIGHTLEG:
            return 0.75f;
            
        case HITGROUP_CHEST:
        case HITGROUP_LEFTARM:
        case HITGROUP_RIGHTARM:
        default:
            return 1.f;
    }
}

/*
 *
 *
 */
static void scale_damage(hit_group_t hitgroup, base_player_t* player, float weapon_armor_ratio, float& current_damage)
{
    current_damage *= get_damage_multiplier(hitgroup);
    
    if(player->get_armor() > 0)
    {
        if(hitgroup == HITGROUP_HEAD)
        {
            if (player->has_helmet())
                current_damage *= (weapon_armor_ratio * 0.5f);
        }
        else
        {
            current_damage *= (weapon_armor_ratio * 0.5f);
        }
    }
}

/*
 *
 *
 */
static bool trace_to_exit(vec3_t& end, trace_t* enter_trace, vec3_t start, vec3_t dir, trace_t* exit_trace, player_t* target)
{
    float distance = 0.0f;
    
    while(distance <= 90.0f)
    {
        distance += 4.0f;
        end = start + dir * distance;
        
        auto point_contents = g_engine_trace->get_point_contents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);
        
        if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
            continue;
        
        auto new_end = end - (dir * 4.0f);
        
        ray_t ray(end, new_end);
        g_engine_trace->trace_ray(ray, MASK_SHOT, 0, exit_trace);
        
        if(exit_trace->m_start_solid && exit_trace->m_surface.m_flags & SURF_HITBOX)
        {
            ray.init(end, start);
            
            trace_filter_t filter;
            filter.m_skip = exit_trace->m_ent;
            
            g_engine_trace->trace_ray(ray, 0x600400B, &filter, exit_trace);
            
            if ((exit_trace->m_fraction < 1.0f || exit_trace->m_all_solid) && !exit_trace->m_start_solid)
            {
                end = exit_trace->m_end_pos;
                return true;
            }
            
            continue;
        }
        
        if (!(exit_trace->m_fraction < 1.0 || exit_trace->m_all_solid || exit_trace->m_start_solid) || exit_trace->m_start_solid)
        {
            if (exit_trace->m_ent)
            {
                if (enter_trace->m_ent && enter_trace->m_ent == (entity_t*)target)
                    return true;
            }
            
            continue;
        }
        
        if (exit_trace->m_surface.m_flags >> 7 & 1 && !(enter_trace->m_surface.m_flags >> 7 & 1))
            continue;
        
        if (exit_trace->m_plane.m_normal.dot(dir) <= 1.0f)
        {
            auto fraction = exit_trace->m_fraction * 4.0f;
            end = end - (dir * fraction);
            
            return true;
        }
    }
    
    return false;
}

/*
 *
 *
 */
static bool handle_bullet_penetration(weapon_info_t* weapon_info, fire_bullet_data_t& data)
{
    surface_data_t* enter_surface_data = g_physics->get_surface_data(data.m_trace.m_surface.m_surface_props);
    int enter_material = enter_surface_data->m_game.m_material;
    float enter_surf_penetration_mod = enter_surface_data->m_game.m_penetration_modifier;
    
    data.m_length += data.m_trace.m_fraction * data.m_lenth_remaining;
    data.m_damage *= powf(weapon_info->m_range_modifier, data.m_length * 0.002f);
    
    if (data.m_length > 3000.f || enter_surf_penetration_mod < 0.1f)
        data.m_count = 0;
    
    if (data.m_count <= 0)
        return false;
    
    vec3_t dummy;
    trace_t trace_exit;

    if(!trace_to_exit(dummy, &data.m_trace, data.m_trace.m_end_pos, data.m_direction, &trace_exit, data.m_target))
        return false;
    
    surface_data_t* exit_surface_data = g_physics->get_surface_data(trace_exit.m_surface.m_surface_props);
    int exit_material = exit_surface_data->m_game.m_material;
    
    float exit_surf_penetration_mod = exit_surface_data->m_game.m_penetration_modifier;
    
    float final_damage_modifier = 0.16f;
    float combined_penetration_modifier = 0.0f;
    
    if ((data.m_trace.m_contents & CONTENTS_GRATE) != 0 || enter_material == 89 || enter_material == 71)
    {
        combined_penetration_modifier = 3.0f;
        final_damage_modifier = 0.05f;
    }
    else
        combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
    
    if (enter_material == exit_material)
    {
        if (exit_material == 87 || exit_material == 85)
            combined_penetration_modifier = 3.0f;
        else if (exit_material == 76)
            combined_penetration_modifier = 2.0f;
    }
    
    float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
    float v35 = (data.m_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weapon_info->m_penetration) * 1.25f);
    float thickness = (trace_exit.m_end_pos - data.m_trace.m_end_pos).length();
    
    thickness *= thickness;
    thickness *= v34;
    thickness /= 24.0f;
    
    float lost_damage = fmaxf(0.0f, v35 + thickness);
    
    if (lost_damage > data.m_damage)
        return false;
    
    if (lost_damage >= 0.0f)
        data.m_damage -= lost_damage;
    
    if (data.m_damage < 1.0f)
        return false;
    
    data.m_src = trace_exit.m_end_pos;
    data.m_count--;
    
    return true;
}

/*
 *
 *
 */
static void trace_line(vec3_t start, vec3_t end, unsigned int mask, entity_t* ignore, trace_t* ptr)
{
    ray_t ray(start, end);
    trace_filter_t filter;
    filter.m_skip = ignore;
    g_engine_trace->trace_ray(ray, mask, &filter, ptr);
}

/*
 *
 *
 */
static bool simulate_fire_bullet(bool team, fire_bullet_data_t& data)
{
    if(!global::weapon)
        return false;
    
    weapon_info_t* weapon_info = global::weapon->get_weapon_info();
    
    if(!weapon_info)
       return false;
    
    data.m_count = 4;
    data.m_length = 0.0f;
    data.m_damage = (float) weapon_info->m_damage;
    
    while (data.m_count > 0 && data.m_damage >= 1.0f)
    {
        data.m_lenth_remaining = weapon_info->m_range - data.m_length;
        
        vec3_t end = data.m_src + data.m_direction * data.m_lenth_remaining;
        
        data.m_trace;
        trace_line(data.m_src, end, MASK_SHOT, global::local, &data.m_trace);
        
        ray_t ray(data.m_src, end + data.m_direction * 40.f);
        g_engine_trace->trace_ray(ray, MASK_SHOT, &data.m_filter, &data.m_trace);
        
        trace_line(data.m_src, end + data.m_direction * 40.f, MASK_SHOT, global::local, &data.m_trace);
        
        if (data.m_trace.m_fraction == 1.0f)
            break;
        
        if (data.m_trace.m_hit_group <= HITGROUP_RIGHTLEG && data.m_trace.m_hit_group > HITGROUP_GENERIC)
        {
            data.m_length += data.m_trace.m_fraction * data.m_lenth_remaining;
            data.m_damage *= powf(weapon_info->m_range_modifier, data.m_length * 0.002f);
            
            player_t* player = (player_t*)data.m_trace.m_ent;
            
            if (team && player->get_team() == global::local->get_team())
                return false;
            
            scale_damage(data.m_trace.m_hit_group, player, weapon_info->m_armor_ratio, data.m_damage);
            
            return true;
        }
        
        if (!handle_bullet_penetration(weapon_info, data))
            break;
    }
    
    return false;
}

/*
 *
 *
 */
int rage_bot_t::get_damage(const vec3_t& point, bool team, player_t* player)
{
    if(!global::local || !global::weapon)
        return -1;
    
    float damage = 0.f;
    vec3_t dst = point;

    fire_bullet_data_t data;
    data.m_src = global::local->get_eye_position();
    data.m_target = player;
    data.m_filter.m_skip = global::local;
    
    qangle_t angles = calculate_angle(data.m_src, dst);

    angle_vectors(angles, data.m_direction);
    
    vec3_t tmp = data.m_direction;
    data.m_direction = tmp.normalise();
    
    if(simulate_fire_bullet(team, data))
        damage = data.m_damage;
    
    return damage;
}
