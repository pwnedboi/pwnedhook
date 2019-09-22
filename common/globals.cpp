/*  globals.cpp
 *
 *
 */
#include "common.h"

namespace global
{
    bool*       send_packet;
    user_cmd_t* cmd;
    player_t*   local;
    weapon_t*   weapon;
    int         choked;
}

settings_t          set;

panel_t*            g_panel;
surface_t*          g_surface;
base_client_dll_t*  g_client;
cvar_t*             g_cvar;
engine_t*           g_engine;
entity_list_t*      g_ent_list;
debug_overlay_t*    g_overlay;
global_vars_t*      g_globals;
client_mode_t*      g_client_mode;
engine_trace_t*     g_engine_trace;
input_system_t*     g_input_system;
game_event_manager2_t*  g_game_events;
model_render_t*     g_model_render;
model_info_t*       g_model_info;
material_system_t*  g_mat_system;
client_state_t*     g_client_state;
input_t*            g_input;
physics_t*          g_physics;

random_int_fn       random_int_func;
random_seed_fn      random_seed_func;
random_float_fn     random_float_func;

vmt_t*              panel_vmt;
vmt_t*              surface_vmt;
vmt_t*              client_vmt;
vmt_t*              client_mode_vmt;
vmt_t*              model_vmt;
vmt_t*              material_vmt;


color_t color_t::white = color_t(255, 255, 255, 255), color_t::black = color_t(0, 0, 0, 255), color_t::pwned = color_t(164, 135, 214, 255);
color_t color_t::green = color_t(148, 213, 90, 255), color_t::blue = color_t(0, 0, 255, 255), color_t::magenta = color_t(230, 40, 240, 255);
