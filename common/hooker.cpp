/*      hooker.cpp
 *
 *
 *
 */
#include "common.h"
#include "hooker.h"
#include "event_listener.h"
#include "chams.h"              // create_materials

event_listner_t* g_listener = nullptr;

/*
 *
 *
 */
static void init_interfaces()
{
    // wait until the game has loaded
    do
    {
        g_panel = create_interface<panel_t>("./bin/osx64/vgui2.dylib", "VGUI_Panel");
    }while(!g_panel);
        
    g_surface   = create_interface<surface_t>("./bin/osx64/vguimatsurface.dylib", "VGUI_Surface");
    g_client    = create_interface<base_client_dll_t>("./csgo/bin/osx64/client_panorama.dylib", "VClient");
    g_cvar      = create_interface<cvar_t>("./bin/osx64/materialsystem.dylib", "VEngineCvar");
    g_engine    = create_interface<engine_t>("./bin/osx64/engine.dylib", "VEngineClient");
    g_ent_list  = create_interface<entity_list_t>("./csgo/bin/osx64/client_panorama.dylib", "VClientEntityList");
    g_overlay   = create_interface<debug_overlay_t>("./bin/osx64/engine.dylib", "VDebugOverlay");
    g_engine_trace  = create_interface<engine_trace_t>("./bin/osx64/engine.dylib", "EngineTraceClient");
    g_input_system  = create_interface<input_system_t>("./bin/osx64/inputsystem.dylib", "InputSystemVersion");
    g_game_events   = create_interface<game_event_manager2_t>("./bin/osx64/engine.dylib", "GAMEEVENTSMANAGER002", true);
    g_model_render  = create_interface<model_render_t>("./bin/osx64/engine.dylib", "VEngineModel");
    g_model_info    = create_interface<model_info_t>("./bin/osx64/engine.dylib", "VModelInfoClient");
    g_mat_system    = create_interface<material_system_t>("./bin/osx64/materialsystem.dylib", "VMaterialSystem");
    g_physics       = create_interface<physics_t>("./bin/osx64/vphysics.dylib", "VPhysicsSurfaceProps");
    
//    pPrediction     = GetInterface<IPrediction>("./csgo/bin/osx64/client_panorama.dylib", "VClientPrediction");
//    pGameMovement   = GetInterface<IGameMovement>("./csgo/bin/osx64/client_panorama.dylib", "GameMovement");
}

/*
 *
 *
 */
static void init_hooks()
{
    g_memory = new memory_manager_t();
    
    g_memory->load_modules();
    
    g_offsets.player_anim_state = g_memory->get_procedure("client_panorama.dylib", SIG_ANIM_STATE_OFFSET, MSK_ANIM_STATE_OFFSET, 0) + 0x3;
    uintptr_t global_vars_ptr   = g_memory->get_pointer("client_panorama.dylib", SIG_GLOBALVARS, MSK_GLOBALVARS, 0x3) + 0x4;
    uintptr_t client_mode_ptr   = g_memory->get_pointer("client_panorama.dylib", SIG_CLIENTMODE, MSK_CLIENTMODE, 0xA) + 0x4;
    uintptr_t send_packet_ptr   = g_memory->get_procedure("engine.dylib", SIG_SENDPACKET, MSK_SENDPACKET, 0x1) + 0x2;
    
    global::send_packet = reinterpret_cast<bool*>(send_packet_ptr);
    g_memory->protect_addr(global::send_packet, 0x1 | 0x2 | 0x4);
    
    uintptr_t get_local_client_ptr  = (uintptr_t)getvtable(g_engine)[12];
    
    void* handle            = dlopen("./csgo/bin/osx64/client_panorama.dylib", RTLD_NOLOAD | RTLD_NOW);
    random_int_func         = reinterpret_cast<random_int_fn>       (dlsym(handle, "RandomInt"));
    random_seed_func        = reinterpret_cast<random_seed_fn>      (dlsym(handle, "RandomSeed"));
    random_float_func       = reinterpret_cast<random_float_fn>     (dlsym(handle, "RandomFloat"));
    dlclose(handle);
    
    get_local_client_fn get_local_client_func = (get_local_client_fn)g_memory->get_absolue_address(get_local_client_ptr + 0x9, 0x1, 0x5);
    
    g_globals       = *(global_vars_t**)(global_vars_ptr);
    g_client_mode   =  (client_mode_t*)(client_mode_ptr);
    g_client_state  =  get_local_client_func(-1);
    g_input         = **(input_t***)(g_memory->get_absolue_address(getvfunc<uintptr_t>(g_client, 16) + 0x4, 0x3, 0x7));
    
    panel_vmt   = new vmt_t(g_panel);
    surface_vmt = new vmt_t(g_surface);
    client_vmt  = new vmt_t(g_client);
    client_mode_vmt = new vmt_t(g_client_mode);
    model_vmt   = new vmt_t(g_model_render);
    material_vmt= new vmt_t(g_mat_system);
    
    g_listener  = new event_listner_t();
}

/*
 *
 *
 */
void cheat_init()
{
    init_interfaces();
    
    init_hooks();
    
    g_netvar.init();
    
    g_offsets.init();
}

/*
 *
 *
 */
static void hook_functions()
{
    panel_vmt->hook((void*)paint_traverse_hook, INDEX_PAINT_TRAVERSE);
    panel_vmt->apply();
    
    surface_vmt->hook((void*)lock_cursor_hook, INDEX_LOCK_CURSOR);
    surface_vmt->apply();
    
    client_vmt->hook((void*)key_event_hook, INDEX_KEY_EVENT);
    client_vmt->hook((void*)frame_stage_notify_hook, INDEX_FRAME_STAGE_NOTIFY);
    client_vmt->apply();
    
    client_mode_vmt->hook((void*)create_move_hook, INDEX_CREATE_MOVE);
    client_mode_vmt->hook((void*)override_view_hook, INDEX_OVERRIDE_VIEW);
    client_mode_vmt->apply();
    
    model_vmt->hook((void*)draw_model_execute_hook, INDEX_DRAW_MODEL_EXE);
    model_vmt->apply();
    
    open_gl_hook();
    
    g_listener->add_listner("player_hurt");
    g_listener->add_listner("bullet_impact");
}

/*
 *
 *
 */
static void prepare_settings()
{
    pwnedhook("injected");
    
    g_engine->get_screen_size(set.screen.w, set.screen.h);
    
    set.visuals.other.hitmarkers.resize(3);
    set.legit.hitboxes.resize(5); // head, chest, stomach, arms, legs
    set.rage.hitboxes.resize(5); // head, chest, stomach, arms, legs
    
    g_chams->create_materials();
}

/*
 *
 *
 */
void cheat_start()
{
    hook_functions();
    
    prepare_settings();
}

void cheat_shutdown()
{
    delete g_memory;
    delete g_listener;
}
