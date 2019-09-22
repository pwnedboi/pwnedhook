/*
 *  input.h
 */
#pragma once

class input_t
{
public:
    
    char    _pad0[0xB4];
    bool    m_camer_intercepting_mouse;
    bool    m_camera_in_thirdperson;
    bool    m_camera_moving_with_mouse;
    vec3_t  m_camera_offset;
    bool    m_camera_distance_move;
    int     m_camera_old_x;
    int     m_camera_old_y;
    int     m_camera_x;
    int     m_camera_y;
    bool    m_camera_is_orthographic;
};
