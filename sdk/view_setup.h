/*
 *  view_setup.h
 */
#pragma once

class view_setup_t
{
public:
    char    _pad0[16];
    int 	m_width;
    int     m_unscaled_width;
    int 	m_height;
    int 	m_unscaled_height;
    char 	_pad20[152];
    float   m_fov;
    float   m_fov_viewmodel;
    vec3_t  m_origin;
    vec3_t  m_angles;
    float   m_z_near;
    float   m_z_far;
    float   m_z_near_viewmodel;
    float   m_z_far_viewmodel;
    float   m_aspect_ratio;
    float   m_near_blur_depth;
    float   m_near_focus_depth;
    float   m_far_focus_depth;
    float   m_far_blur_depth;
    float   m_near_blur_radius;
    float 	m_far_blur_radius;
    float   m_do_fquality;
    int     m_motion_blur_mode;
};
