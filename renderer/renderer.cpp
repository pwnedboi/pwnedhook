/*  renderer.cpp
 *
 *
 */
#include "common.h"
#include "renderer.h"

renderer_t* g_render = new renderer_t();

HFONT renderer_t::verdana12;

/*
 *
 *
 */
void renderer_t::draw_box(int x, int y, int w, int h, color_t color)
{
    g_surface->set_color(color);
    g_surface->draw_outlined_rect(x, y, x + w, y + h);
}

/*
 *
 *
 */
void renderer_t::draw_box_filled(int x, int y, int w, int h, color_t color)
{
    g_surface->set_color(color);
    g_surface->draw_filled_rect(x, y, x + w, y + h);
}

/*
 *
 *  draws a box with an outline
 */
void renderer_t::draw_box_outlined(int x, int y, int w, int h, int thickness, color_t color, color_t color_out)
{
    draw_box_filled(x - thickness, y - thickness, w + (thickness * 2), h + (thickness * 2), color_out);
    draw_box_filled(x, y, w, h, color);
}

/*
 *
 *
 */
void renderer_t::draw_box_gradient(int x, int y, int w, int h, color_t color_start, color_t color_end, bool hori)
{
    // verticle
    if(!hori)
    {
        this->draw_box_filled(x, y, w, h, color_start);
        
        unsigned char r  = color_end.r();
        unsigned char g = color_end.g();
        unsigned char b  = color_end.b();
        
        for (int i = 0; i < h; i++)
        {
            float fi    = i, fh = h;
            float a     = fi / fh;
            int ia      = a * 255;
            
            this->draw_box_filled(x, y + i, w, 1, color_t(r, g, b, ia));
        }
    }
    // horizontal
    else
    {
        float r = (float)(color_end.r() - color_start.r()) / (float)w;
        float g = (float)(color_end.g() - color_start.g()) / (float)w;
        float b = (float)(color_end.b() - color_start.b()) / (float)w;
        
        for (float i = 0.f; i < w; i++)
        {
            color_t gradient = color_t(color_start.r() + (r * i), color_start.g() + (g * i), color_start.b() + (b * i), color_start.a());
            this->draw_box_filled(x + i, y, 1, h, gradient);
        }
    }
}

/*
 *
 *
 */
void renderer_t::draw_line(int x, int y, int xx, int yy, color_t color)
{
    g_surface->set_color(color);
    g_surface->draw_line(x, y, xx, yy);
}

/*
 *
 *
 */
void renderer_t::draw_string(int x, int y, HFONT font, string str, color_t color, bool centered_x, bool centered_y)
{
    vec2_t ts = vec2_t();
    
    if(centered_x || centered_y)
        ts = get_text_size(font, str);
    
    if(centered_x)
        x -= (ts.x / 2);
    
    if(centered_y)
        y -= (ts.y / 2);
    
    g_surface->set_text_font(font);
    g_surface->set_text_color(color);
    g_surface->set_text_pos(x, y);
    g_surface->draw_text(string_wstring(str).c_str(), (int)str.length());
}

/*
 *
 *
 */
void renderer_t::draw_circle(vec2_t center, int radius, int points, color_t color, bool hollow)
{
    if(hollow)
    {
        
    }
    else
    {
        vector<vertex_t> vertices;
        float step = (float)M_PI * 2.0f / points;
        
        for (float a = 0; a < (M_PI * 2.0f); a += step)
            vertices.push_back(vertex_t(vec2_t(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));
        
        draw_polygon(vertices, color);
    }
}

/*
 *
 *
 */
void renderer_t::draw_triangle(vec2_t p1, vec2_t p2, vec2_t p3, color_t color, bool down, bool hollow)
{
    /*         p2
               *
              / \
             /   \
            /     \
        p1 *-------* p3
     */
    
    vector<vertex_t> verts = {vertex_t(p1), vertex_t(p2), vertex_t(p3)};
    
    draw_polygon(verts, color);
}

/*
 *
 *
 */
void renderer_t::draw_polygon(vector<vertex_t> verts, color_t color, bool hollow)
{
    if(hollow)
    {
        const size_t c = verts.size();
        int x[c], y[c];
        
        for(int i = 0; i < c; i++)
        {
            vec2_t v = verts.at(i).m_position;
            
            x[i] = v.x;
            y[i] = v.y;
        }
        
        g_surface->draw_poly_line(x, y, (int)c);
    }
    else
    {
        static int tex_id = 0;
        static u_char buf[4] = {255, 255, 255, 255};
        
        if(!g_surface->is_texture_id_valid(tex_id))
            tex_id = g_surface->create_new_texture_id(true);
        
        g_surface->set_texture_rgba(tex_id, buf, 1, 1);
        g_surface->set_color(color);
        g_surface->set_texture(tex_id);
        g_surface->draw_textured_polygon((int)verts.size(), verts.data());
    }
}

// misc

/*
 *
 *
 */
vec2_t renderer_t::get_text_size(HFONT font, string str)
{
    wstring wstr = string_wstring(str);
    
    int x, y;

    g_surface->get_text_size(font, wstr.c_str(), x, y);
    
    return vec2_t(x, y);    
}

/*
 *
 *
 */
wstring renderer_t::string_wstring(const string& str)
{
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    
    try
    {
        return converter.from_bytes(str);
    }
    catch(range_error e)
    {
        wostringstream s;
        s << str.c_str();
        return s.str();
    }
}

/*
 *
 *
 */
HFONT renderer_t::create_font(string font_name, int size, int flags)
{
    HFONT font = g_surface->create_hfont();
    g_surface->set_font_glyph_set(font, font_name.c_str(), size, 150, 0, 0, flags);
    return font;
}
