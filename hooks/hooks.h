/*
 *  hooks.h
 */
#pragma once

// todo :
//  sdl_poll_event

#define print_hook() do{ static bool print = false; if(!print) { pwnedhook(std::string(__FUNCTION__) + " hooked"); print = true; } }while(0)
#define get_orig(vmt, type, index) vmt->get_orignal_method<type>(index)

#define INDEX_PAINT_TRAVERSE        42
#define INDEX_LOCK_CURSOR           67
#define INDEX_KEY_EVENT             21
#define INDEX_FRAME_STAGE_NOTIFY    37
#define INDEX_CREATE_MOVE           25
#define INDEX_DRAW_MODEL_EXE        21
#define INDEX_OVERRIDE_VIEW         19

typedef void(*paint_traverse_fn)(void*, VPANEL, bool, bool);
typedef int (*key_event_fn)(void*, int, int, const char*);
typedef void(*frame_stage_notify_fn)(void*, frame_stage_t);
typedef bool(*create_move_fn)(void*, float, user_cmd_t*);
typedef void(*draw_model_execute_fn)(void*, void*, void*, const model_render_info_t&, matrix3x4_t*);
typedef void(*override_view_fn)(void*, view_setup_t*);

void paint_traverse_hook(void* thisptr, VPANEL panel, bool repaint, bool allow_force);
void lock_cursor_hook(void* thisptr);
int  key_event_hook(void* thisptr, int event_code, int key_code, const char* cur_binding);
void frame_stage_notify_hook(void* thisptr, frame_stage_t frame_stage);
bool create_move_hook(void* thisptr, float sample, user_cmd_t* cmd);
void draw_model_execute_hook(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix);
void override_view_hook(void* thisptr, view_setup_t* setup);
void open_gl_hook();
