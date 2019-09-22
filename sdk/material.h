/*
 *  material.h
 */
#pragma once

enum material_var_flags_t
{
    MATERIAL_VAR_DEBUG = ( 1 << 0 ),
    MATERIAL_VAR_NO_DEBUG_OVERRIDE = ( 1 << 1 ),
    MATERIAL_VAR_NO_DRAW = ( 1 << 2 ),
    MATERIAL_VAR_USE_IN_FILLRATE_MODE = ( 1 << 3 ),
    MATERIAL_VAR_VERTEXCOLOR = ( 1 << 4 ),
    MATERIAL_VAR_VERTEXALPHA = ( 1 << 5 ),
    MATERIAL_VAR_SELFILLUM = ( 1 << 6 ),
    MATERIAL_VAR_ADDITIVE = ( 1 << 7 ),
    MATERIAL_VAR_ALPHATEST = ( 1 << 8 ),
    // MATERIAL_VAR_UNUSED  = (1 << 9),
    MATERIAL_VAR_ZNEARER = ( 1 << 10 ),
    MATERIAL_VAR_MODEL = ( 1 << 11 ),
    MATERIAL_VAR_FLAT = ( 1 << 12 ),
    MATERIAL_VAR_NOCULL = ( 1 << 13 ),
    MATERIAL_VAR_NOFOG = ( 1 << 14 ),
    MATERIAL_VAR_IGNOREZ = ( 1 << 15 ),
    MATERIAL_VAR_DECAL = ( 1 << 16 ),
    // MATERIAL_VAR_ENVMAPSPHERE = ( 1 << 17 ), // OBSOLETE
    // MATERIAL_VAR_UNUSED  = (1 << 18),
    // MATERIAL_VAR_ENVMAPCAMERASPACE = ( 1 << 19 ), // OBSOLETE
    MATERIAL_VAR_BASEALPHAENVMAPMASK = ( 1 << 20 ),
    MATERIAL_VAR_TRANSLUCENT = ( 1 << 21 ),
    MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = ( 1 << 22 ),
    // MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = ( 1 << 23 ), // OBSOLETE
    MATERIAL_VAR_OPAQUETEXTURE = ( 1 << 24 ),
    // MATERIAL_VAR_ENVMAPMODE = ( 1 << 25 ), // OBSOLETE
    MATERIAL_VAR_SUPPRESS_DECALS = ( 1 << 26 ),
    MATERIAL_VAR_HALFLAMBERT = ( 1 << 27 ),
    MATERIAL_VAR_WIREFRAME = ( 1 << 28 ),
    MATERIAL_VAR_ALLOWALPHATOCOVERAGE = ( 1 << 29 ),
    MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = ( 1 << 30 ),
    MATERIAL_VAR_VERTEXFOG = ( 1 << 31 ),
};

class material_t
{
public:
    
    const char* get_name()
    {
        typedef const char* (*o_get_name)(void*);
        return getvfunc<o_get_name>(this, 0)(this);
    }
    
    const char* get_texture_group_name()
    {
        typedef const char* (*o_get_texture_group_name)(void*);
        return getvfunc<o_get_texture_group_name>(this, 1)(this);
    }
    
    void alpha_modulate(float alpha)
    {
        typedef void (*o_alpha_modulate)(void*, float);
        return getvfunc<o_alpha_modulate>(this, 27)(this, alpha);
    }
    
    void color_modulate(float r, float g, float b)
    {
        typedef void (*o_color_modulate)(void*, float, float, float);
        return getvfunc<o_color_modulate>(this, 28)(this, r, g, b);
    }
    
    void color_modulate(color_t color)
    {
        typedef void (*o_color_modulate)(void*, float, float, float);
        return getvfunc<o_color_modulate>(this, 28)(this, color.r() / 255.f, color.g() / 255.f, color.b() / 255.f);
    }
    
    void increment_reference_count()
    {
        typedef void (*o_increment_reference_count)(void*);
        return getvfunc<o_increment_reference_count>(this, 10)(this);
    }
    
    void set_material_var_flag(material_var_flags_t flag, bool on)
    {
        typedef void (*o_set_material_var_flag)(void*, material_var_flags_t, bool);
        return getvfunc<o_set_material_var_flag>(this, 29)(this, flag, on);
    }
    
    float get_alpha_modulation()
    {
        typedef float (*o_get_alpha_modulation)(void*);
        return getvfunc<o_get_alpha_modulation>(this, 44)(this);
    }
    
    void get_color_modulation(float* r, float* g, float* b)
    {
        typedef void (*o_get_color_modulation)(void*, float*, float*, float*);
        return getvfunc<o_get_color_modulation>(this, 45)(this, r, g, b);
    }
};
