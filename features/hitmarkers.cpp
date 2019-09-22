/*
 *  hitmarkers.cpp
 */
#include "common.h"
#include "visuals.h"
#include "renderer.h"

static long last_time_stamp = 0;

struct hitmarker_t
{
    int  m_damage;
    long m_timestamp;
};

std::vector<hitmarker_t> g_damages = {};

void visuals_t::draw_hitmarkers()
{
    if(!set.visuals.other.hitmarkers.at(0) && !set.visuals.other.hitmarkers.at(2))
        return;
    
    if(!global::local || !global::local->is_alive())
        return;
    
    int duration = 1500;
    long now = util_timestamp();
    
    long diff = last_time_stamp + duration - now;
    
    if(diff <= 0)
        return;
    
    color_t color = color_t::white;
    
    color.set_a(std::min(color.a(), (int)(diff * color.a() / duration * 2)));
    
    int sides[4][2] = { {-1, -1}, {1, 1}, {-1, 1}, {1, -1} };
    
    // see bellow
    // const int size = 10;
    // const int gap  = 4;
    
    if(set.visuals.other.hitmarkers.at(0))
    {
        for (auto& it : sides)
            g_render->draw_line(set.screen.w/2+(4*it[0]), set.screen.h/2+(4*it[1]), set.screen.w/2+(10*it[0]), set.screen.h/2+(10*it[1]),color);
    }
    
    // should we draw damages?
    if(!set.visuals.other.hitmarkers.at(2))
        return;
    
    float text_height = g_render->get_text_size(renderer_t::verdana12, "[pwned]]").y;
    
    for (unsigned int i = 0; i < g_damages.size(); i++)
    {
        long timestamp = g_damages.at(i).m_timestamp;
        long hit_dif = timestamp + duration - now;
        
        if (hit_dif <= 0)
        {
            g_damages.erase(g_damages.begin() + i);
            continue;
        }
        
        vec2_t pos = vec2_t(set.screen.w / 2 + 10 + 4, set.screen.h / 2 - 10 - text_height * i + 4);
        
        color.set_a(255);
        color.set_a(std::min(color.a(), (int)(hit_dif * color.a() / duration * 2)));
        
        g_render->draw_string(pos.x, pos.y, renderer_t::verdana12, to_string(g_damages.at(i).m_damage), color);
    }
}

//
void hitmarker_event_player_hurt(game_event_t* event)
{
    if (!set.visuals.other.hitmarkers.at(0) && !set.visuals.other.hitmarkers.at(1) && !set.visuals.other.hitmarkers.at(2))
        return;
    
    if (!g_engine->is_in_game())
        return;
    
    int hurt_player_id  = event->get_int("userid");
    int attacker_id     = event->get_int("attacker");
    
    if (g_engine->get_player_for_user_id(hurt_player_id) == g_engine->get_local_player())
        return;
    
    if (g_engine->get_player_for_user_id(attacker_id) != g_engine->get_local_player())
        return;
    
    player_t* hurt_player = (player_t*)g_ent_list->get_entity(g_engine->get_player_for_user_id(hurt_player_id));
    
    if (!hurt_player || !global::local)
        return;
    
    // only show hitmarkers on enemies
    if (hurt_player->get_team() == global::local->get_team())
        return;
    
    // play sound
    if(set.visuals.other.hitmarkers.at(1))
        // skeet sound
        g_engine->execute_client_cmd("play buttons\\arena_switch_press_02.wav");
    
    long now = util_timestamp();
    last_time_stamp = now;
    g_damages.insert(g_damages.begin(), {event->get_int("dmg_health"), now});
}
