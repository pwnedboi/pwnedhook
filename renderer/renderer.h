/*
 * renderer.h
 */
#pragma once

class renderer_t
{
private:
    
    wstring string_wstring(const string& str);
    
public:
    
    static HFONT verdana12;
    
public:
    
    void draw_box(int x, int y, int w, int h, color_t color);
    void draw_box_filled(int x, int y, int w, int h, color_t color);
    void draw_box_outlined(int x, int y, int w, int h, int thickness, color_t color, color_t color_out);
    void draw_box_gradient(int x, int y, int w, int h, color_t color_start, color_t color_end, bool hori = false);
    void draw_line(int x, int y, int xx, int yy, color_t color);
    void draw_string(int x, int y, HFONT font, string str, color_t color, bool centered_x = false, bool centered_y = false);
    void draw_circle(vec2_t center, int radius, int points, color_t color, bool hollow = false);
    void draw_triangle(vec2_t p1, vec2_t p2, vec2_t p3, color_t color, bool down = false, bool hollow = false);
    void draw_polygon(vector<vertex_t> verts, color_t color, bool hollow = false);
    
    vec2_t get_text_size(HFONT font, string str);
    
    HFONT create_font(string font_name, int size, int flags);
    
};

extern renderer_t* g_render;
