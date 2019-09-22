/*          movement.cpp
 *
 *
 */
#include "common.h"
#include "movement.h"

movement_t* g_movement = new movement_t();

void movement_t::bhop()
{
    if(!global::local || !global::cmd)
        return;
    
    if(!set.misc.bhop)
        return;
    
    static bool jump[2] = {false, false};
    
    if (jump[1] && !jump[0])
    {
        global::cmd->m_buttons |= IN_JUMP;
        jump[1] = false;
    }
    else if (global::cmd->m_buttons & IN_JUMP)
    {
        if (global::local->get_flags() & FL_ONGROUND)
        {
            jump[1] = false;
            jump[0] = false;
        }
        else
        {
            global::cmd->m_buttons &= ~IN_JUMP;
            jump[0] = false;
        }
    }
    else
    {
        jump[0] = false;
        jump[1] = false;
    }
}

/*
 *
 *  auto strafe
 */
void movement_t::strafe()
{
    if(!global::local || !global::cmd)
        return;
    
    if(!set.misc.strafe)
        return;
    
    if(global::cmd->m_buttons & IN_JUMP && !(global::local->get_flags() & FL_ONGROUND))
    {
        if (global::local->get_velocity().length_2d() == 0 && (global::cmd->m_forward_move == 0 && global::cmd->m_side_move == 0))
        {
            global::cmd->m_forward_move = 450.f;
        }
        else if (global::cmd->m_forward_move == 0 && global::cmd->m_side_move == 0)
        {
            if (global::cmd->m_mousedx > 0 || global::cmd->m_mousedx < -0)
            {
                global::cmd->m_side_move = global::cmd->m_mousedx < 0.f ? -450.f : 450.f;
            }
            else
            {
                float temp1 = 5850.f / global::local->get_velocity().length_2d();
                global::cmd->m_forward_move = temp1;
                
                if (global::cmd->m_forward_move > 450)
                    global::cmd->m_forward_move = 450;
                
                float temp = 450 - (temp1 - global::cmd->m_forward_move);
                
                if (temp < 450)
                    temp = 450;
                
                global::cmd->m_side_move = (global::cmd->m_command_number % 2) == 0 ? -temp : temp;
            }
        }
    }
}
