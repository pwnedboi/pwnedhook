/*      util.cpp
 *
 *
 */
#include "common.h"
#include "util.h"

/*
 *  sdk definitions, todo : move elsewhere
 */
base_weapon_t* base_player_t::get_weapon()
{
    void* handle = this->get_weapon_handle();
    
    if(!handle)
        return nullptr;
    
    return (base_weapon_t*)g_ent_list->get_entity_from_handle(handle);
}

/*
 *  cheat_print
 *  Prints a message to the csgo console
 */
void cheat_print(const string& msg, string prefix, color_t col)
{
    prefix = "[" + prefix + "] ";
    g_cvar->console_print_color(col, "%s", prefix.c_str());
    g_cvar->console_print("%s\n", msg.c_str());
}

/*
 *  util_get_player_info
 *  Returns a player_info_t for a given player
 */
player_info_t util_get_player_info(player_t* player)
{
    player_info_t info;
    
    if(!player)
        return info;
    
    g_engine->get_player_info(player->get_index(), &info);
    
    return info;
}

// is_visible, todo : bool skip_ent, pass thru players n shit

/*
 *
 *
 */
bool util_is_point_visible(vec3_t point)
{
    if(!global::local || !g_engine_trace)
        return false;
    
    ray_t ray(global::local->get_eye_position(), point);
    trace_t trace;
    trace_filter_t filter(global::local);
    
    g_engine_trace->trace_ray(ray, 0x4600400b, &filter, &trace);
    
    return (trace.m_fraction > 0.97f);
}

/*
 *
 *
 */
bool util_is_player_visible(player_t* player)
{
    if(!global::local || !g_engine_trace || !player)
        return false;
    
    ray_t ray(global::local->get_eye_position(), player->get_eye_position());
    trace_t trace;
    trace_filter_t filter(global::local);
    
    g_engine_trace->trace_ray(ray, 0x4600400b, &filter, &trace);
    
    return (trace.m_ent == player || trace.m_fraction > 0.99f);
}

/*
 *
 *
 */
bool util_is_hitbox_visible(player_t* player, int hitbox)
{
    if(!global::local || !g_engine_trace || !player)
        return false;
    
    ray_t ray(global::local->get_eye_position(), get_hitbox_position(player, hitbox));
    trace_t trace;
    trace_filter_t filter(global::local);
    
    g_engine_trace->trace_ray(ray, 0x4600400b, &filter, &trace);

    return (trace.m_ent == player || trace.m_fraction > 0.99f);
}

/*
 *
 *
 */
long util_timestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

/*
 *
 *
 */
std::string util_execute_cmd(const std::string& cmd, bool pop)
{
    std::string result;
    std::array<char, 128> buffer;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    
    if (!pipe)
        throw std::runtime_error("");
    
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    
    if(pop)
        result.pop_back();
    
    return result;
}

/*
 *  to_color
 *  Returns the color_t value
 */
color_t ImColor::to_color()
{
    return color_t(Value.x * 255.f, Value.y * 255.f, Value.z * 255.f, Value.w * 255.f);
}

/*
 *
 *
 */
std::string get_weapon_name(weapon_t* weapon)
{
    if(!weapon)
        return "Unknown";
    
    int id = weapon->get_item_definition_index();
    
    switch (id)
    {
            /* pistols */
        case 4:
            return "Glock";
        case 2:
            return "Dualies";
        case 36:
            return "P250";
        case 30:
            return "Tec 9";
        case 1:
            return "Deagle";
        case 32:
            return "P2000";
        case 3:
            return "Five Seven";
        case 64:
            return "Revolver";
        case 63:
            return "CZ75";
        case 61:
            return "USP";
            
            /*heavy*/
        case 35:
            return "Nova";
        case 25:
            return "XM1014";
        case 29:
            return "Sawed Off";
        case 14:
            return "M249";
        case 28:
            return "Negev";
        case 27:
            return "Mag 7";
            
            /*smgs*/
        case 17:
            return "Mac 10";
        case 33:
            return "MP7";
        case 23:
            return "MP5";
        case 24:
            return "UMP45";
        case 19:
            return "P90";
        case 26:
            return "Bizon";
        case 34:
            return "MP9";
            
            /*rifles*/
        case 10:
            return "Famas";
        case 16:
            return "M4A1";
        case 40:
            return "Scout";
        case 8:
            return "AUG";
        case 9:
            return "AWP";
        case 38:
            return "Scar 20";
        case 13:
            return "Galil";
        case 7:
            return "AK47";
        case 39:
            return "SG553";
        case 11:
            return "G3SG1";
        case 60:
            return "M4A1-S";
            
            /*grenades*/
        case 46:
        case 48:
            return "Molotov";
        case 44:
            return "Grenade";
        case 43:
            return "Flash";
        case 45:
            return "Smoke";
        case 47:
            return "Decoy";
            
            /*other*/
        case 31:
            return "Zeus";
        case 49:
            return "Bomb";
        default:
            return "Knife";
    }
}
