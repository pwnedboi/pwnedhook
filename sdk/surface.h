/*
 *  surface.h
 */
#pragma once

typedef void(*lock_cursor_fn)(void*);

// todo : add more funcs
class surface_t : public app_system_t
{
public:
    
    void set_color(int r, int g, int b, int a)
    {
        typedef void (*oDrawSetColor)(void*, int, int, int, int);
        return getvfunc<oDrawSetColor>(this, 14)(this, r, g, b, a);
    }
    
    void set_color(color_t color)
    {
        typedef void (*oDrawSetColor)(void*, int, int, int, int);
        return getvfunc<oDrawSetColor>(this, 14)(this, color.r(), color.g(), color.b(), color.a());
    }
    
    void draw_filled_rect(int x0, int y0, int x1, int y1)
    {
        typedef void (*oDrawFilledRect)(void*, int, int, int, int);
        return getvfunc<oDrawFilledRect>(this, 16)(this, x0, y0, x1, y1);
    }
    
    void draw_outlined_rect(int x0, int y0, int x1, int y1)
    {
        typedef void (*oDrawOutlinedRect)(void*, int, int, int, int);
        return getvfunc<oDrawOutlinedRect>(this, 18)(this, x0, y0, x1, y1);
    }
    
    void draw_line(int x0, int y0, int x1, int y1)
    {
        typedef void (*oDrawLine)(void*, int, int, int, int);
        return getvfunc<oDrawLine>(this, 19)(this, x0, y0, x1, y1);
    }
    
    void draw_poly_line(int *x, int *y, int count)
    {
        typedef void (*oDrawPolyLine)(void*, int*, int*, int);
        return getvfunc<oDrawPolyLine>(this, 20)(this, x, y, count);
    }
    
    void set_text_font(HFONT font)
    {
        typedef void (*oDrawSetTextFont)(void*, unsigned long long);
        return getvfunc<oDrawSetTextFont>(this, 23)(this, font);
    }
    
    void set_text_color(color_t color)
    {
        typedef void (*oDrawSetTextColor)(void*, int, int, int, int);
        return getvfunc<oDrawSetTextColor>(this, 24)(this, color.r(), color.g(), color.b(), color.a());
    }
    
    void set_text_color(int r, int g, int b, int a)
    {
        typedef void (*oDrawSetTextColor)(void*, int, int, int, int);
        return getvfunc<oDrawSetTextColor>(this, 24)(this, r, g, b, a);
    }
    
    void set_text_pos(int x, int y)
    {
        typedef void (*oDrawSetTextPos)(void*, int, int);
        return getvfunc<oDrawSetTextPos>(this, 26)(this, x, y);
    }
    
    void draw_text(const wchar_t* text, int textLen)
    {
        typedef void (*oDrawPrintText)(void*, const wchar_t*, int, int);
        return getvfunc<oDrawPrintText>(this, 28)(this, text, textLen, 0);
    }
    
    void set_texture_rgba(int id, unsigned char const* color, int w, int h)
    {
        typedef void (*oDrawSetTextureRGBA)(void*, int, unsigned char const*, int, int);
        return getvfunc<oDrawSetTextureRGBA>(this, 37)(this, id, color, w, h);
    }
    
    void set_texture(int id)
    {
        typedef void (*oDrawSetTexture)(void*, int);
        return getvfunc<oDrawSetTexture>(this, 38)(this, id);
    }
    
    void draw_textured_rect(int x0, int y0, int x1, int y1)
    {
        typedef void (*oDrawTexturedRect)(void*, int, int, int, int);
        return getvfunc<oDrawTexturedRect>(this, 41)(this, x0, y0, x1, y1);
    }
    
    bool is_texture_id_valid(int id)
    {
        typedef bool (*oIsTextureIDValid)(void*, unsigned int);
        return getvfunc<oIsTextureIDValid>(this, 42)(this, id);
    }
    
    int create_new_texture_id(bool procedural)
    {
        typedef int (*oCreateNewTextureID)(void*, bool);
        return getvfunc<oCreateNewTextureID>(this, 43)(this, procedural);
    }
    
    HFONT create_hfont()
    {
        typedef HFONT (*oCreateFont)(void*);
        return getvfunc<oCreateFont>(this, 71)(this);
    }
    
    void set_font_glyph_set(HFONT& font, const char* font_name, int tall, int weight, int blur, int scanlines, int flags)
    {
        typedef void (*oSetFontGlyphSet)(void*, unsigned long long, const char*, int, int, int, int, int, int, int);
        return getvfunc<oSetFontGlyphSet>(this, 72)(this, font, font_name, tall, weight, blur, scanlines, flags, 0, 0);
    }
    
    void get_text_size(unsigned long long font, const wchar_t *text, int &wide, int &tall)
    {
        typedef void (*oGetTextSize)(void*, unsigned long long font, const wchar_t *text, int &wide, int &tall);
        return getvfunc<oGetTextSize>(this, 79)(this, font, text, wide, tall);
    }
    
    void draw_outlined_circle(int x0, int y0, int x1, int y1)
    {
        typedef void(*oDrawOutlinedCircle)(void*, int, int, int, int);
        return getvfunc<oDrawOutlinedCircle>(this, 103)(this, x0, y0, x1, y1);
    }
    
    void draw_textured_polygon(int vtxCount, vertex_t* vtx, bool bClipVertices = true)
    {
        typedef void (*oDrawTexturedPolygon)(void*, int, vertex_t*, bool);
        return getvfunc<oDrawTexturedPolygon>(this, 106)(this, vtxCount, vtx, bClipVertices);
    }
    
    void unlock_cursor()
    {
        typedef void(*UnlockCursorFn)(void*);
        return getvfunc<UnlockCursorFn>(this, 66)(this);
    }
    
    void lock_cursor()
    {
        return getvfunc<lock_cursor_fn>(this, 67)(this);
    }
    
    bool is_cursor_locked()
    {
        typedef bool(*IsCursorLockedFn)(void*);
        return getvfunc<IsCursorLockedFn>(this, 107)(this);
    }
    
    void play_sound(const char* file)
    {
        typedef void(*PlaySoundFn)(void*, const char*);
        return getvfunc<PlaySoundFn>(this, 82)(this, file);
    }
};
