/*
 *  user_cmd.h
 */
#pragma once

struct user_cmd_t
{
    virtual     ~user_cmd_t() {};
    int         m_command_number;
    int         m_tick_count;
    qangle_t    m_view_angles;
    vec3_t      m_aim_direction;
    float       m_forward_move;
    float       m_side_move;
    float       m_up_move;
    int         m_buttons;
    byte        m_impulse;
    int         m_weapon_select;
    int         m_weapon_subtype;
    int         m_random_seed;
    short       m_mousedx;
    short       m_mousedy;
    bool        m_has_been_predicted;
    qangle_t    m_head_angles;
    vec3_t      m_head_offset;
};
