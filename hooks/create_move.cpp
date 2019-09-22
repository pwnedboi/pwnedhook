/*      create_move.cpp
 *
 *
 */
#include "common.h"
#include "antiaim.h"
#include "ragebot.h"
#include "movement.h"
#include "backtrack.h"

// todo : clean up
bool create_move_hook(void* thisptr, float sample, user_cmd_t* cmd)
{
    print_hook();
    
    bool ret = client_mode_vmt->get_original_method<create_move_fn>(INDEX_CREATE_MOVE)(thisptr, sample, cmd);
    
    if(!cmd)
        return ret;
    
    if(!cmd->m_command_number)
        return ret;
    
    global::cmd = cmd;
    user_cmd_t orig_cmd = *cmd;
    
    if(!global::local)
        global::local = (player_t*)g_ent_list->get_entity(g_engine->get_local_player());
    
    if(!global::weapon)
        global::weapon = global::local->get_weapon();
    
    // cancel crouch delay
    cmd->m_buttons |= IN_BULLRUSH;
    
    
    // features
    if(global::local)
    {
        // code that affects movement is called before predition
        
        g_movement->bhop();
        
        g_movement->strafe();
        
        // prediction start
        
        // maybe move to framestage        
        
        if(global::weapon)
        {
            g_rage->aimbot();
            
            // g_legit->aimbot();
        }
        
        g_backtrack->backtrack_player(nullptr);
        
        // prediction end
    }
    
    if(global::send_packet)
    {
        if(*global::send_packet)
        {
            global::choked++;
        }
        else
        {
            global::choked = 0;
        }
    }
    
    cmd->m_view_angles.clamp();
    
    return ret;
}
