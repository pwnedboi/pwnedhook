/*      menu.cpp
 *
 *
 */
#include "common.h"
#include "menu.h"
#include "config.h"

namespace
{
    ImColor col_background(14, 14, 14, 255), col_border1(70, 70, 70, 255), col_border2(41, 42, 42, 255);
}

menu_t::~menu_t()
{
    delete draw;
}

void menu_t::init()
{
    draw = new im_renderer_t();
    
    draw->init_fonts();
    
    x = y = 100;
    w = 500;
    h = 400;
    tab = 2;
    
    reset_offset();
}

void menu_t::render()
{
    draw->start();
    
    draw_window();
    draw_tabs();
    reset_offset();
    
    draw->finish();
}

void menu_t::move()
{
    bool down = ImGui::GetIO().MouseDown[0];
    static std::vector<bool> down_last = {};
    const int magic = 15;
            
    if(draw->in_area(x + 10, y + 10, 125, h - 20) && (down || (down_last.size() > 0 && down_last.size() < magic)))
    {
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        x += delta.x;
        y += delta.y;
    }

    if(down || down_last.size() < magic)
        down_last.push_back(true);
}

void menu_t::draw_window()
{
    // window
    draw->draw_box_outlined(x - 4, y - 4, w + 8, h + 8, 2, col_border1, col_border2);
    draw->draw_box_filled(x, y, w, h, col_background);
    
    // tab box
    draw->draw_box(x + 10, y + 10, 125, h - 20, col_border1);
    
    // controls
    draw->draw_box(x + 10 + 125 + 10, y + 10, 345, h - 20, col_border1);
}

void menu_t::draw_tabs()
{
    const std::vector<std::string> tab_names = {"rage", "antiaim", "legit", "visuals", "movement", "misc", "skins", "players"};
    
    for(int i = 0; i < tab_names.size(); i++)
    {
        ImColor col = ImColor(140, 140, 140, 255);
        
        // if active then use active col
        if(tab == i)
            col = ImColor(157, 209, 70, 255);
            // col = set.menu.color;
        
        // draw label
        draw->draw_string(x + 22, y + 20 + (i * 15), tab_names.at(i).c_str(), Fonts::small, col);
        
        // change to this tab if clicked
        if(draw->in_area(x + 19, y + 17 + (i * 15), 75, 15) && ImGui::GetIO().MouseClicked[0])
            tab = i;
    }

    // draw current tab
    int counter = 0;
    if(tab == counter++)
        tab_rage();
    else if(tab == counter++)
        tab_antiaim();
    else if(tab == counter++)
        tab_legit();
    else if(tab == counter++)
        tab_visuals();
    else if(tab == counter++)
        tab_movement();
    else if(tab == counter++)
        tab_misc();
    else if(tab == counter++)
        tab_skins();
    else if(tab == counter++)
        tab_players();
    
    // render these last so they are on top of everything
    // when theyre open
    render_combos();
    // render this last so its ontop of combos,
    // bc it looks like a lil window ontop of the current one
    render_color_pickers();
}

void menu_t::tab_rage()
{
    set_side(side_left);
    checkbox("aimbot", &set.rage.aimbot);
    slider_i("fov", {0, 180}, &set.rage.fov, false, "º");
    combo_multi("target hitboxes", {"head", "chest", "stomach", "arms", "legs"}, &set.rage.hitboxes, &opened.rage_hitboxes);
    checkbox("slient", &set.rage.silent);
    checkbox("autowall", &set.rage.autowall, opened.rage_hitboxes);
    slider_i("min damage", {0, 100}, &set.rage.min_damage, opened.rage_hitboxes, "hp");
    checkbox("backtrack", &set.rage.backtrack);
    
    set_side(side_right);
}

void menu_t::tab_antiaim()
{
    set_side(side_left);
    checkbox("slow walk", &set.antiaim.slow_walk);
    checkbox("fake duck", &set.antiaim.fake_duck);
    
    set_side(side_right);
}

void menu_t::tab_legit()
{
    set_side(side_left);
    checkbox("aimbot", &set.legit.aimbot);
    slider_i("fov", {0, 90}, &set.legit.fov, false, "º");
    slider_i("smoothing", {0, 100}, &set.legit.smooth, false, "%");    
    checkbox("backtrack", &set.legit.backtrack);
    
    set_side(side_right);
}

void menu_t::tab_visuals()
{
    set_side(side_left);
    combo("team flags", {"enemy", "teammates", "all"}, &set.visuals.team_flags, &opened.team_flags);
    checkbox("visible", &set.visuals.visible, opened.team_flags);
    checkbox("box", &set.visuals.player.box, opened.team_flags);
    color_picker(&set.colors.players.box, &opened.picker_box);
    checkbox("name", &set.visuals.player.name);
    checkbox("health bar", &set.visuals.player.health);
    combo("bottom bar", {"off", "armor", "ammo"}, &set.visuals.player.bot_bar, &opened.bot_bar);
    checkbox("skeleton", &set.visuals.player.skeleton);
    checkbox("weapon", &set.visuals.player.equipment);
    checkbox("equipment", &set.visuals.player.equipment);
    checkbox("snap lines", &set.visuals.player.snap_lines);
    checkbox("chams", &set.visuals.chams.players);
    color_picker(&set.colors.chams.players, &opened.picker_chams_players);
    checkbox("through walls", &set.visuals.chams.behind_walls);
    color_picker(&set.colors.chams.behind_walls, &opened.picker_behind_walls);
    combo("chams type", {"textured", "flat"}, &set.visuals.chams.player_type, &opened.chams_players);
    combo("backtrack", {"off", "lines", "chams"}, &set.visuals.player.backtrack, &opened.backtrack);
    
    set_side(side_right);
    combo_multi("hitmarkers", {"crosshair", "sound", "damage"}, &set.visuals.other.hitmarkers, &opened.hitmarkers);
    checkbox("bomb timer", &set.visuals.other.bomb_timer, opened.hitmarkers);
    checkbox("remove smoke", &set.visuals.other.remove_smoke, opened.hitmarkers);
    slider_f("flashbang effect", {0.f, 100.f}, &set.visuals.other.flash_alpha, false, "%", 0);
    checkbox("remove scope", &set.visuals.other.remove_scope);
}

void menu_t::tab_movement()
{
    set_side(side_left);
    checkbox("bhop", &set.misc.bhop);
    checkbox("auto strafe", &set.misc.strafe);
    
    set_side(side_right);
}

void menu_t::tab_misc()
{
    set_side(side_left);
    slider_i("override fov", {0, 60}, &set.misc.fov, false, "º");
    checkbox("remove view punch", &set.misc.remove_view_punch);
    
    set_side(side_right);
    combo("config", g_config.get_configs(), &set.config, &opened.config);
    if(button("save"))
        g_config.save();
    if(button("load"))
        g_config.load();
}

void menu_t::tab_skins()
{
    set_side(side_left);
    
    set_side(side_right);
}

void menu_t::tab_players()
{
    set_side(side_left);
    
    set_side(side_right);
}
