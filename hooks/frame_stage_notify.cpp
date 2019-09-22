/*  frame_stage_notify.cpp
 *
 *
 */
#include "common.h"

#include "ragebot.h"
#include "visuals.h"
#include "backtrack.h"

void frame_stage_notify_hook(void* thisptr, frame_stage_t frame_stage)
{
    print_hook();
    
    bool in_game = g_engine->is_in_game();
    qangle_t orig_view, orig_aim;
    
    if(frame_stage == FRAME_START)
    {
        
    }
    
    if(frame_stage == FRAME_NET_UPDATE_START)
    {
        
    }
    
    if(frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
    {
        if(in_game)
        {
            g_visuals->remove_flash();
        }
    }
    
    if(frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
    {
        if(in_game)
        {
            g_visuals->remove_smoke();
        }
    }
    
    if(frame_stage == FRAME_NET_UPDATE_END)
    {
        
    }
    
    if(frame_stage == FRAME_RENDER_START)
    {
        if(global::local && in_game && global::local->is_alive())
        {
            if(set.misc.thirdperson)
            {
                // todo
                // *local->get_view_angles() = fake / real
            }
            
            g_rage->remove_recoil();
            
            if(set.misc.remove_view_punch)
            {
                orig_aim    = *global::local->get_aim_punch_angle();
                orig_view   = *global::local->get_view_punch_angle();
                
                global::local->get_aim_punch_angle()->init();
                global::local->get_view_punch_angle()->init();
            }
        }
        
        g_backtrack->store();
    }
    
    if(frame_stage == FRAME_RENDER_END)
    {
        
    }
    
    client_vmt->get_original_method<frame_stage_notify_fn>(INDEX_FRAME_STAGE_NOTIFY)(thisptr, frame_stage);
        
    if(in_game && set.misc.remove_view_punch)
    {
        *global::local->get_aim_punch_angle()  = orig_aim;
        *global::local->get_view_punch_angle() = orig_view;
    }
}
