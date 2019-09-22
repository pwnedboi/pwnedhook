/*
 *  legitbot.cpp
 *  todo : recode
 */
#include "common.h"
#include "legitbot.h"

legitbot_t* g_legit = new legitbot_t();

static vec3_t screen_mid(0, 0, 0);

static std::vector<hitbox_t> get_target_hitboxes()
{
    /*
    std::vector<hitbox_t> hitboxes;
    
    if(set.legit.hitboxes.at(0))
    {
        hitboxes.push_back(HITBOX_HEAD);
        hitboxes.push_back(HITBOX_NECK);
    }
    
    if(set.legit.hitboxes.at(1))
    {
        hitboxes.push_back(HITBOX_CHEST);
        hitboxes.push_back(HITBOX_UPPER_CHEST);
    }
    
    if(set.legit.hitboxes.at(2))
    {
        hitboxes.push_back(HITBOX_PELVIS);
        hitboxes.push_back(HITBOX_BODY);
    }
    
    if(set.legit.hitboxes.at(3))
    {
        hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
        hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
    }
    
    if(set.legit.hitboxes.at(4))
    {
        hitboxes.push_back(HITBOX_LEFT_THIGH);
        hitboxes.push_back(HITBOX_RIGHT_THIGH);
        hitboxes.push_back(HITBOX_LEFT_CALF);
        hitboxes.push_back(HITBOX_RIGHT_CALF);
    }
    */
    
    std::vector<hitbox_t> hitboxes = {
        HITBOX_HEAD,
        HITBOX_NECK,
        HITBOX_PELVIS,
        HITBOX_BODY,
        // HITBOX_THORAX,
        HITBOX_CHEST,
        // HITBOX_UPPER_CHEST,
        HITBOX_RIGHT_THIGH,
        HITBOX_LEFT_THIGH,
        HITBOX_RIGHT_CALF,
        HITBOX_LEFT_CALF,
        // HITBOX_RIGHT_FOOT,
        // HITBOX_LEFT_FOOT,
        // HITBOX_RIGHT_HAND,
        // HITBOX_LEFT_HAND,
        HITBOX_RIGHT_UPPER_ARM,
        // HITBOX_RIGHT_FOREARM,
        HITBOX_LEFT_UPPER_ARM,
        // HITBOX_LEFT_FOREARM
    };
    
    return hitboxes;
}

int legitbot_t::find_closest_hitbox(player_t* player)
{
    auto    hitboxes    = get_target_hitboxes();
    float   best_dist   = MAXFLOAT;
    int     best_hitbox = -1;

    for(auto hitbox : hitboxes)
    {
        if(!util_is_hitbox_visible(player, hitbox))
            continue;

        vec3_t world_pos, hitbox_pos = get_hitbox_position(player, hitbox);
        
        if(!world_to_screen(hitbox_pos, world_pos))
            continue;
        
        float dist = screen_mid.dist_to(world_pos);

        if(dist < best_dist)
        {
            best_dist = dist;
            best_hitbox = hitbox;
        }
    }

    return best_hitbox;
}

void legitbot_t::find_closest_target()
{
    float best_dist = MAXFLOAT;
    
    for(int i = 1; i < g_globals->m_max_clients; i++)
    {
        player_t* player = g_ent_list->get_player(i);
        
        if(!player)
            continue;
        
        if(!player->is_player())
            continue;
        
        if(!player->is_alive() || player->is_dormant() || player->is_immune())
            continue;
        
        int target_hitbox = find_closest_hitbox(player);
        vec3_t world_pos, hitbox_pos = get_hitbox_position(player, target_hitbox);
        
        if(!world_to_screen(hitbox_pos, world_pos))
            continue;
        
        float dist = screen_mid.dist_to(world_pos);
        
        if(dist < best_dist)
        {
            best_dist = dist;
            m_target_index = i;
            m_target_hitbox = target_hitbox;
        }
    }
}

/*
 *
 *
 */
vec3_t legitbot_t::get_aim_position(player_t* player, int hitbox)
{
    vec3_t aim_pos = vec3_t(0, 0, 0);
    
    // if backtrack
    //     get_closest_record
    
    if(aim_pos.is_zero())
        aim_pos = get_hitbox_position(player, hitbox);
    
    // todo : prediction
    
    // todo : make better
    if(!aim_pos.is_zero())
        aim_pos += player->get_velocity() * g_globals->m_interval_per_tick;
    
    return aim_pos;
}

// if we can see it we can hit it
// todo : aim thru players
bool legitbot_t::can_hit_target()
{
    if(!m_target)
        return false;
    
    if(m_target_hitbox == -1)
        return false;
    
    if(!util_is_hitbox_visible(m_target, m_target_hitbox))
        m_target_hitbox = find_closest_hitbox(m_target);
    
    if(m_target_hitbox == -1)
        return false;
    
    if(!util_is_hitbox_visible(m_target, m_target_hitbox))
        return false;
    
    return true;
}

/*
 *
 *
 */
void legitbot_t::aimbot()
{
    // todo
    if(!set.legit.aimbot)
        return;
    
    if(screen_mid.is_zero())
        screen_mid  = vec3_t(set.screen.w / 2, set.screen.h / 2, 0);
    
    if(!can_hit_target())
    {
        find_closest_target();
        pwnedhook("new target : " + std::to_string(m_target_index));
    }
    
    if(m_target_index > 0)
        m_target = g_ent_list->get_player(m_target_index);
    
    if(!m_target)
        return;
    
    qangle_t view_angles;
    g_engine->get_view_angles(view_angles);
    
    vec3_t   aim_pos = get_aim_position(m_target, m_target_hitbox);
    qangle_t aim_ang = calculate_angle(global::local->get_eye_position(), aim_pos);
    
    float fov = get_fov(view_angles, aim_ang);
    
    if(fov > set.legit.fov)
        return;
    
    qangle_t smooth_ang = aim_ang;
    
    // smoothing
    // https://www.unknowncheats.me/forum/c-and-c/151887-doing-better-aim-smoothing.html
    // https://github.com/stevemk14ebr/BF4-AtomBot/blob/47168a85ab4d3311cff8756539740f7c0dd7c52f/BF4%20AtomBot/Hack%20Core/AimbotSmoother.h
    if(set.legit.smooth > 0)
        smooth_ang = view_angles + (aim_ang - view_angles) / set.legit.smooth;
    
    if(global::cmd->m_buttons & IN_ATTACK)
    {
        if(!aim_pos.is_zero())
        {
            global::cmd->m_view_angles = smooth_ang;
            g_engine->set_view_angles(global::cmd->m_view_angles);
        }
    }
}
