/*
 *  debug_overlay.h
 */
#pragma once

// todo : add more funcs
class debug_overlay_t
{
public:
    
    bool screen_position(const vec3_t& in, vec3_t& out)
    {
        typedef bool (*o_screen_position)(void*, const vec3_t&, vec3_t&);
        return getvfunc<o_screen_position>(this, 11)(this, in, out);
    }
    
    void add_line_overlay(const vec3_t& origin, const vec3_t& dest, int r, int g, int b, bool no_depth, float duration)
    {
        typedef void(*o_add_line_overlay)(void*, const vec3_t&, const vec3_t&, int, int, int, bool, float);
        return getvfunc<o_add_line_overlay>(this, 4)(this, origin, dest, r, g, b, no_depth, duration);
    }
};
