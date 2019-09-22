/*
 *  base_entity.h
 */
#pragma once

#define sdk_get_offset(type, table, netvar) *(type*)((uintptr_t)this + g_offsets.table.netvar);
#define sdk_get_pointer(type, table, netvar) (type*)((uintptr_t)this + g_offsets.table.netvar);

class collidable_t
{
public:
    
    virtual void pad0();
    virtual const vec3_t& obb_min() const;
    virtual const vec3_t& obb_max() const;
    
};

class base_handle_t;

class handle_entity_t
{
public:
    
    virtual ~handle_entity_t() {};
    virtual void set_ref_ehandle(const base_handle_t& handle) = 0;
    virtual const base_handle_t& get_ref_ehandle() const = 0;
    
};

class client_unknown_t : public handle_entity_t
{
    
};

class client_renderable_t
{
public:
    
    virtual ~client_renderable_t() {};
    
    model_t* get_model()
    {
        typedef model_t* (*og_get_model)(void*);
        return getvfunc<og_get_model>(this, 8)(this);
    }
    
    bool setup_bones(matrix3x4_t* matrix, int max_bones, int bone_mask, float cur_time = 0)
    {
        typedef bool (*o_setup_bones)(void*, matrix3x4_t*, int, int, float);
        return getvfunc<o_setup_bones>(this, 13)(this, matrix, max_bones, bone_mask, cur_time);
    }
};

class client_networkable_t
{
public:
    
    virtual ~client_networkable_t() {};
    
    void release()
    {
        typedef void (*o_release)(void*);
        return getvfunc<o_release>(this, 1)(this);
    }
    
    client_class_t* get_client_class()
    {
        typedef client_class_t* (*o_get_client_class)(void*);
        return getvfunc<o_get_client_class>(this, 2)(this);
    }
    
    void pre_data_update(data_update_type_t udpate_type)
    {
        typedef void (*o_pre_data_update)(void*, data_update_type_t);
        return getvfunc<o_pre_data_update>(this, 6)(this, udpate_type);
    }
    
    void set_destroyed_on_recreate_entities()
    {
        typedef void (*o_set_destroyed_on_recreate_entities)(void*);
        return getvfunc<o_set_destroyed_on_recreate_entities>(this, 13)(this);
    }
};

class client_thinkable_t
{
public:
    virtual ~client_thinkable_t() {}
};

class client_entity_t : public client_unknown_t, public client_renderable_t, public client_networkable_t, public client_thinkable_t
{
public:
    
    virtual ~client_entity_t() {};
    
    int get_index()
    {
        return *(int*)((uintptr_t)this + 0x94);
    }
};

/*
 *  base_entity_t
 */

class base_entity_t : public client_entity_t
{
public:
    
    client_networkable_t* get_networkable()
    {
        return (client_networkable_t*)((uintptr_t)this + 0x10);
    }
    
    int get_id()
    {
        return *(int*)((uintptr_t)this + 0x94);
    }
    
    bool is_dormant()
    {
        return *(bool*)((uintptr_t)this + 0x125);
    }
    
    int get_team()
    {
        return sdk_get_offset(int, base_entity, m_team);
    }
    
    collidable_t* get_collidable()
    {
        return sdk_get_pointer(collidable_t, base_entity, m_collision);
    }
    
    vec3_t get_origin()
    {
        return sdk_get_offset(vec3_t, base_entity, m_origin);
    }
    
    float get_simulation_time()
    {
        return sdk_get_offset(float, base_entity, m_simulation_time);
    }
};

typedef base_entity_t entity_t;
