/*
 *  engine_trace.h
 *  https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/IEngineTrace.h
 */
#pragma once

/*
 *  structs
 */

struct ray_t
{
    ray_t() {}
    ray_t(vec3_t start, vec3_t end) { init(start, end); }
    
    vec3_aligned_t      m_start;
    vec3_aligned_t      m_delta;
    vec3_aligned_t      m_start_offset;
    vec3_aligned_t      m_extents;
    const matrix3x4_t*  m_world_axis_transform;
    bool                m_is_ray;
    bool                m_is_swept;
    
    void init(vec3_t start, vec3_t end)
    {
        m_delta         = vec3_aligned_t(end - start);
        m_is_swept      = (m_delta.length_sqr() != 0);
        m_extents.zero();
        m_world_axis_transform = NULL;
        m_is_ray        = true;
        m_start_offset.zero();
        m_start         = start;
    }
};

struct plane_t
{
    vec3_t          m_normal;
    float           m_dist;
    unsigned char   m_type;
    unsigned char   m_sign_bits;
    unsigned char   m_pad[2];
};

struct csurface_t
{
    const char*         m_name;
    short               m_surface_props;
    unsigned short      m_flags;
};

struct trace_t
{
    vec3_t          m_start_pos;
    vec3_t          m_end_pos;
    plane_t         m_plane;
    float           m_fraction;
    int             m_contents;
    unsigned int    m_disp_flags;
    bool            m_all_solid;
    bool            m_start_solid;
    float           m_fraction_left_solid;
    csurface_t      m_surface;
    hit_group_t     m_hit_group;
    short           m_physics_bone;
    unsigned short  m_world_surface_index;
    base_entity_t*  m_ent;
    int             m_hitbox;
    
    inline bool did_hit() const { return m_fraction < 1.0f || m_all_solid || m_start_solid; }
    bool did_hit_world() const { return m_ent->get_index() == 0; }
    bool did_hit_non_world_entity() const { return m_ent != nullptr && !did_hit_world(); }
};

/*
 *  trace filter
 */

class i_trace_filter_t
{
public:
    virtual bool should_hit_entity(base_entity_t* ent, int mask)
    {
        return !(ent == m_skip);
    }
    virtual trace_type_t get_trace_type()
    {
        return m_type;
    }
    
    void*           m_skip;
    int             m_collision_group;
    trace_type_t    m_type;
};

class trace_filter_t : public i_trace_filter_t
{
public:
    
    trace_filter_t(base_entity_t* ent = nullptr, trace_type_t type = TRACE_EVERYTHING, int col_group = 0)
    {
        m_skip = ent;
        // m_collision_group = col_group; // COLLISION_GROUP_NONE
        m_type = type;
    }
    
    virtual bool should_hit_entity(base_entity_t* ent, int mask)
    {
        return !(ent == m_skip);
    }
};

class trace_filter_world_only_t : public trace_filter_t
{
public:
    bool should_hit_entity(base_entity_t* ent, int mask)
    {
        return false;
    }
};

/*
 *  engine trace
 */

class engine_trace_t
{
public:
    // Returns the contents mask + entity at a particular world-space position
    virtual int get_point_contents(const vec3_t& abs_pos, int mask = MASK_ALL, handle_entity_t** ent = nullptr) = 0;
    
    // Returns the contents mask of the world only @ the world-space position (static props are ignored)
    virtual int get_point_contents_world_only(const vec3_t& abs_pos, int mask = MASK_ALL) = 0;
    
    // Get the point contents, but only test the specific entity. This works
    // on static props and brush models.

    // If the entity isn't a static prop or a brush model, it returns CONTENTS_EMPTY and sets
    // bFailed to true if bFailed is non-null.
    virtual int get_point_contents_collideable(collidable_t* collide, const vec3_t& abs_pos) = 0;
    
    // Traces a ray against a particular entity
    virtual void clip_ray_to_entity(const ray_t& ray, unsigned int mask, handle_entity_t* ent, trace_t* trace) = 0;
    
    // Traces a ray against a particular entity
    virtual void clip_ray_to_collideable(const ray_t& ray, unsigned int mask, collidable_t* collide, trace_t* trace) = 0;
    
    // A version that simply accepts a ray (can work as a traceline or tracehull)
    virtual void trace_ray(const ray_t& ray, unsigned int mask, i_trace_filter_t* filter, trace_t* trace) = 0;
};
