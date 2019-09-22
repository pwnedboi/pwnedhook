/*
 * physics.h
 */
#pragma once

struct surface_physics_params_t
{
    float m_friction;
    float m_elasticity;
    float m_density;
    float m_thickness;
    float m_dampening;
};

struct surface_audio_params_t
{
    float   m_reflectivity;             // like elasticity, but how much sound should be reflected by this surface
    float   m_hardness_factor;           // like elasticity, but only affects impact sound choices
    float   m_roughness_factor;          // like friction, but only affects scrape sound choices
    float   m_rough_threshold;           // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
    float   m_hard_threshold;            // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
    float   m_hard_velocity_threshold;    // collision velocity > this causes "hard" impacts, < this causes "soft" impacts
    float   m_high_pitch_occlusion;       //a value betweeen 0 and 100 where 0 is not occluded at all and 100 = silent (except for reflected sounds)
    float   m_mid_pitch_occlusion;
    float   m_low_pitch_occlusion;
};

struct surface_sound_names_t
{
    unsigned short m_walk_step_left;
    unsigned short m_walk_step_light;
    unsigned short m_run_step_left;
    unsigned short m_run_step_right;
    unsigned short m_impact_soft;
    unsigned short m_impact_hard;
    unsigned short m_scrape_smooth;
    unsigned short m_scrape_rough;
    unsigned short m_bullet_impact;
    unsigned short m_rolling;
    unsigned short m_break_sound;
    unsigned short m_strain_sound;
};

struct surface_sound_handles_t
{
    short m_walk_step_left;
    short m_walk_step_light;
    short m_run_step_left;
    short m_run_step_right;
    short m_impact_soft;
    short m_impact_hard;
    short m_scrape_smooth;
    short m_scrape_rough;
    short m_bullet_impact;
    short m_rolling;
    short m_break_sound;
    short m_strain_sound;
};

struct surface_game_props_t
{
public:
    float   m_max_speed_factor;
    float   m_jump_factor;
    char    m_pad0[0x4];
    float   m_penetration_modifier;
    float   m_damage_modifier;
    unsigned short m_material;
    char    m_pad1[0x3];
};

struct surface_data_t
{
    surface_physics_params_t    m_physics;
    surface_audio_params_t      m_audio;
    surface_sound_names_t       m_sounds;
    surface_game_props_t        m_game;
    surface_sound_handles_t     m_sound_handles;
};

// physics_surface_props_t
class physics_t
{
public:
    virtual ~physics_t(void) {}
    
    virtual int parse_surface_data(const char* file_name, const char* text_file) = 0;
    virtual int surface_prop_count(void) const = 0;
    virtual int get_surface_index(const char* prop_name) const = 0;
    virtual void get_physics_properties(int index, float* density, float* thickness, float* friction, float* elasticity) const = 0;
    virtual surface_data_t* get_surface_data(int index) = 0;
    virtual const char* get_string(unsigned short table_index) const = 0;
    virtual const char* get_prop_name(int index) const = 0;
    virtual void set_world_material_index_table(int* map, int map_size) = 0;
    virtual void get_physics_parameters(int index, surface_physics_params_t* out_params) const = 0;
};
