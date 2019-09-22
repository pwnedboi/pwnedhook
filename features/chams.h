/*
 *  chams.h
 */
#pragma once

class chams_manager_t
{
public:
    
    void create_materials(bool reload = false);
    
    void hands(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix);
    void player(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix);
    void weapons(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix);
    
    void fake_lag(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix);
    void backtrack(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix);
    void fake_angles(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix);
    
};

extern chams_manager_t* g_chams;
