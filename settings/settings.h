/*
 *  settings.h
 */
#pragma once

#include "imgui.h"

typedef std::vector<bool> multi;

struct settings_t
{
    // current config
    int config  = 0;
    
    struct
    {
        bool    open    = false;
        int     key     = -1;
    }menu;
    
    // screen size
    struct
    {
        int w, h;
    }screen;
    
    // feature settings
    
    struct
    {
        bool  aimbot    = false;
        multi hitboxes  = {};
        int   fov       = 0;    // 0 - 180
        int   smooth    = 0;    // 0 - 100, percent, 0 = off
        int   rcs_x     = 0;    // 0 - 100, percent, 0 = off
        int   rcs_y     = 0;
        bool  backtrack = false;
    }legit;
    
    struct
    {
        bool  aimbot    = false;
        multi hitboxes  = {};
        int   fov       = 0;    // 0 - 180
        bool  silent    = false;
        bool  backtrack = false;
        bool  autowall  = false;
        int   min_damage= 0;    // 0 - 100
        bool  remove_recoil = false; // remove aim punch
    }rage;
    
    struct
    {
        bool slow_walk  = false;
        bool fake_duck  = false;
        
        // todo
        bool enabled    = false;
        bool desync     = false;
        int  yaw        = 0;       
        int  pitch      = 0;
    }antiaim;
    
    struct
    {
        int  fov = 0;   // fov changer
        
        bool bhop = false;
        bool strafe = false;
        
        bool remove_view_punch  = false;
        bool remove_aim_punch   = false;
        
        // todo
        bool thirdperson    = false;
        int  view_fov       = 0; // view model fov
        bool clantag        = false;
        bool show_ranks     = false;
    }misc;
    
    struct
    {
        // flags
        bool visible    = false; // visible check
        int  team_flags = 0;     // 0 = enemy only, 1 = team only, 2 = all
        
        // player visuals
        struct
        {
            bool box        = false;
            bool name       = false;
            bool health     = false;
            int  bot_bar    = 0;        // 0 = off, 1 = armor, 2 = ammo... todo
            bool armor      = false;
            bool equipment  = false;
            
            // todo :
            bool weapon     = false;
            bool skeleton   = false;
            bool snap_lines = false;
            int  backtrack  = 0;        // 0 = off, 1 = line, 2 = chams
        }player;
        
        struct
        {
            bool players        = false;
            bool behind_walls   = false;    // same as visible flag, todo : remove
            int  player_type    = 0;        // 0 = flat, 1 = textured... todo
        }chams;
        
        struct
        {
            multi   hitmarkers; // crosshair, sound, damage
            bool    bomb_timer  = false;
            
            // removals
            float     flash_alpha = 0.f;
            bool    remove_smoke= false;
            bool    remove_scope= false;
            
            // todo:
            bool    weapons     = false; // droppped
            int     crosshair   = 0;     // 0 = off, 1 = static, 2 = recoil, 3 = spread
            bool    sniper_crosshair = false;
        }other;
    }visuals;
    
    struct
    {
        ImColor menu = ImColor(255, 255, 255, 255);
        
        struct
        {
            // color_t box = color_t::white, offscreen = color_t::blue;
            ImColor box = ImColor(255, 255, 255);
        }players;
        
        struct
        {
            ImColor players         = ImColor(255, 255, 255, 255);
            ImColor behind_walls    = ImColor(255, 255, 255, 255);
        }chams;
    }colors;
};
