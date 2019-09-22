/*  chams.cpp
 *
 *
 */
#include "common.h"
#include "chams.h"

#include <unistd.h>
#include <sys/proc_info.h>
#include <libproc.h>

chams_manager_t* g_chams = new chams_manager_t;

static bool g_mats_created = false;

/*
 *  materials
 */
static material_t *mat_flat_vis = nullptr, *mat_flat_ign = nullptr, *mat_text_vis = nullptr, *mat_text_ign = nullptr;

/*
 *
 *
 */
static bool does_file_exist(const std::string& str)
{
    std::fstream f(str);
    return f.good();
}

static std::string get_path()
{
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
    proc_pidpath(getpid(), pathbuf, sizeof(pathbuf));
    std::string path = pathbuf;
    path.erase(find(path.rbegin(), path.rend(), '/').base(), path.end());
    return path;
    return util_execute_cmd("pwd");
}

/*
 *
 *
 */
static material_t* create_material(string filename, bool flat, bool ignorez, bool wireframe)
{
    std::string type     = flat ? "VertexLitGeneric" : "UnlitGeneric";
    std::string mat_path = get_path() + "csgo/materials/";
    std::string path     = mat_path + filename + ".vmt";
    
    if(!does_file_exist(path))
    {
        std::stringstream ss;
        
        ss << "\"" + type + "\"\n" << endl;
        ss << "{\n" << endl;
        ss << "    \"$basetexture\" \"VGUI/white_additive\"\n" << endl;
        ss << "    \"$nofog\" \"1\"\n" << endl;
        ss << "    \"$ignorez\" \"" + to_string(ignorez) + "\"\n" << endl;
        ss << "    \"$wireframe\" \""+ to_string(wireframe) +"\"\n" << endl;
        ss << "    \"$halflambert\" \"1\"\n" << endl;
        ss << "}\n" << endl;
        
        ofstream f;
        f.open(path);
        f << ss.str();
        f.close();
        
    }

    material_t* mat = g_mat_system->find_material(filename.c_str(), TEXTURE_GROUP_MODEL);    
    mat->increment_reference_count();
    
    return mat;
}

/*
 *
 *
 */
void chams_manager_t::create_materials(bool reload)
{
    // if we want to relaod then create again
    if(reload)
        g_mats_created = false;
    
    // if we havent created yet
    if(!g_mats_created)
    {
        mat_flat_vis = create_material("pwned_flat",        true,   false, false);
        mat_flat_ign = create_material("pwned_flat_ign",    true,   true,  false);
        mat_text_vis = create_material("pwned_text",        false,  false, false);
        mat_text_ign = create_material("pwned_text_ign",    false, 	true,  false);
        
        g_mats_created = true;
        
        pwnedhook("Chams materials created");
    }
}

/*
 *
 *
 */
void chams_manager_t::hands(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix)
{
    
}

/*
 *
 *
 */
void chams_manager_t::player(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix)
{
    if(!set.visuals.chams.players)
        return;
    
    player_t* player = (player_t*)g_ent_list->get_entity(model_info.m_entity_index);
    
    if(!player || !global::local)
        return;
    
    // todo : remove, just temp
    if(player == global::local)
        return;
    
    if(!player->is_player())
        return;
    
    if(!player->is_alive() || player->is_dormant())
        return;
    
    if(player->get_team() == global::local->get_team() && set.visuals.team_flags == 0)
        return;
    
    if(player->get_team() != global::local->get_team() && set.visuals.team_flags == 1)
        return;
    
    color_t col_vis = set.colors.chams.players.to_color();
    color_t col_ign = set.colors.chams.behind_walls.to_color();
    
    material_t* mat_vis = nullptr;
    material_t* mat_ign = nullptr;
    
    switch(set.visuals.chams.player_type)
    {
        case 0:
            mat_vis = mat_flat_vis;
            mat_ign = mat_flat_ign;
            break;
        case 1:
        default:
            mat_vis = mat_text_vis;
            mat_ign = mat_text_ign;
            break;
    }
    
    if(!mat_vis || !mat_ign)
    {
        pwnedhook("no chams mats");
        return;
    }
    
    if(player == global::local)
    {
        // alpha modulate 0.7
    }
    else
    {
        float alpha = 1.f;
        
        if(set.visuals.chams.behind_walls)
        {
            mat_ign->color_modulate(col_ign);
            mat_ign->alpha_modulate(alpha);
            g_model_render->forced_material_override(mat_ign);
            model_vmt->get_original_method<draw_model_execute_fn>(INDEX_DRAW_MODEL_EXE)(thisptr, context, state, model_info, matrix);
        }
        
        mat_vis->color_modulate(col_vis);
        mat_vis->alpha_modulate(alpha);
        g_model_render->forced_material_override(mat_vis);
        // dont need to call this again cause we're calling it in draw_model_execute
        // model_vmt->get_original_method<draw_model_execute_fn>(INDEX_DRAW_MODEL_EXE)(thisptr, context, state, model_info, matrix);
    }
}

/*
 *
 *
 */
void chams_manager_t::weapons(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix)
{
    
}

/*
 *
 *
 */
void chams_manager_t::fake_lag(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix)
{
    
}

/*
 *
 *
 */
void chams_manager_t::backtrack(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix)
{
    
}

/*
 *
 *
 */
void chams_manager_t::fake_angles(void* thisptr, void* context, void* state, const model_render_info_t& model_info, matrix3x4_t* matrix)
{
    
}
