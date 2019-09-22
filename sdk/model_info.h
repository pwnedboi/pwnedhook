/*
 *   model_info.h
 */
#pragma once

// todo rewrite

class  quaternion_t;

class radian_euler_t
{
public:
    
    inline radian_euler_t(void) { }
    
    inline radian_euler_t(float X, float Y, float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }
    
    inline radian_euler_t(quaternion_t const &q);    // evil auto type promotion!!!
    inline radian_euler_t(qangle_t const &angles);   // evil auto type promotion!!!
    
    // Initialization
    inline void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
    {
        x = ix;
        y = iy;
        z = iz;
    }
    
    // conversion to qangle_t
    qangle_t to_qangle(void) const;
    
    bool is_valid() const;
    void invalidate();
    
    inline float* base() { return &x; }
    inline const float* base() const { return &x; }
    
    // array access...
    float operator[](int i) const;
    float &operator[](int i);
    
    float x, y, z;
};

class quaternion_t                // same data-layout as engine's vec4_t,
{                               // which is a float[4]
public:
    
    inline quaternion_t(void) { }
    inline quaternion_t(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw) { }
    inline quaternion_t(radian_euler_t const& angle);    // evil auto type promotion!!!
    
    inline void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f)
    {
        x = ix;
        y = iy;
        z = iz;
        w = iw;
    }
    
    bool is_valid() const;
    void invalidate();
    
    bool operator==(const quaternion_t &src) const;
    bool operator!=(const quaternion_t &src) const;
    
    float* base() { return (float*)this; }
    
    const float* base() const { return (float*)this; }
    
    // array access...
    float operator[](int i) const;
    float &operator[](int i);
    
    float x, y, z, w;
};

struct studio_bone_t
{
    int             m_name_index;
    
    int             m_parent;               // parent bone
    int             m_bone_controller[6];   // bone controller index, -1 == none
    
    // default values
    vec3_t          m_pos;
    quaternion_t    m_quat;
    radian_euler_t  m_rot;
    
    // compression scale
    vec3_t          m_pos_scale;
    vec3_t          m_rot_scale;
    
    matrix3x4_t     m_pose_to_bone;
    quaternion_t    m_alignment;
    int             m_flags;
    int             m_proc_type;
    int             m_proc_index;       // procedural rule
    mutable int     m_physics_bone;     // index into physically simulated bone
    
    int             m_surface_prop_name_index;    // index into string table for property name
    
    int             m_contents;                 // See BSPFlags.h for the contents flags
    int             m_surface_prop_lookup;      // this index must be cached by the loader, not saved in the file
    int             m_unused[7];                // remove as appropriate
    
    inline char* const get_name(void) const
    {
        return ((char*)this) + m_name_index;
    }
    
    inline void* get_prodecure() const
    {
        if (m_proc_index == 0)
            return NULL;
        else
            return (void*)(((unsigned char*)this) + m_proc_index);
    };
    
    inline char* const get_surface_prop_name(void) const
    {
        return ((char*)this) + m_surface_prop_name_index;
    }
    
    inline int get_surface_prop(void) const
    {
        return m_surface_prop_lookup;
    }
};

struct studio_bbox_t
{
    int     m_bone;
    int     m_group;
    vec3_t  m_bbmin;
    vec3_t  m_bbmax;
    int     m_hitbox_name_index;
    int     m_pad[3];
    float   m_radius;
    int     m_pad2[4];
    
    char* get_hitbox_name()
    {
        if (m_hitbox_name_index == 0)
            return NULL;
        
        return ((char*)this) + m_hitbox_name_index;
    }
};

struct studio_hitbox_set_t
{
    int m_name_index;
    
    inline char* const get_name() const
    {
        return ((char*)this) + m_name_index;
    }
    
    int m_num_hitboxes;
    int m_hitbox_index;
    
    inline studio_bbox_t* get_hitbox(int i) const
    {
        return (studio_bbox_t*) (((unsigned char*)this) + m_hitbox_index) + i;
    };
};

struct studio_hdr_t
{
    int     m_id;
    int     m_version;
    int     m_checksum;         // this has to be the same in the phy and vtx files to load!
    char    m_name[64];
    int     m_length;
    
    vec3_t  m_eyeposition;      // ideal eye position
    vec3_t  m_illumposition;    // illumination center
    vec3_t  m_hull_min;         // ideal movement hull size
    vec3_t  m_hull_max;
    vec3_t  m_view_bbmin;       // clipping bounding box
    vec3_t  m_view_bbmax;
    
    int     m_flags;
    int     m_num_bones;        // bones
    int     m_bone_index;
    
    inline studio_bone_t* get_bone(int i) const
    {
        // Assert(i >= 0 && i < numbones);
        return (studio_bone_t*)(((unsigned char*)this) + m_bone_index) + i;
    };
    
    int remap_seq_bone(int seq, int local_bone) const;      // maps local sequence bone to global bone
    int remap_anim_bone(int anim, int local_bone) const;    // maps local animations bone to global bone
    
    int m_num_bone_controllers;        // bone controllers
    int m_bone_controller_index;
    int m_num_hitbox_sets;
    int m_hitbox_set_index;
    
    // Look up hitbox set by index
    studio_hitbox_set_t* get_hitbox_set(int i) const
    {
        // Assert(i >= 0 && i < numhitboxsets);
        return (studio_hitbox_set_t*)(((unsigned char*)this) + m_hitbox_set_index) + i;
    };
    
    // Calls through to hitbox to determine size of specified set
    inline studio_bbox_t* get_hitbox(int hitbox, int set_index) const
    {
        const studio_hitbox_set_t* set = get_hitbox_set(set_index);
        
        if (!set)
            return nullptr;
        
        return set->get_hitbox(hitbox);
    };
    
    // Calls through to set to get hitbox count for set
    inline int get_hitbox_count(int set) const
    {
        const studio_hitbox_set_t* s = get_hitbox_set(set);
        
        if (!s)
            return 0;
        
        return s->m_num_hitboxes;
    };
    
    // file local animations? and sequences
    //private:
    int m_num_local_anim;       // animations/poses
    int m_local_anim_index;     // animation descriptions
    int m_num_local_seq;        // sequences
    int m_local_seq_index;
    
    //public:
    bool sequences_available() const;
    
    int get_num_sequences() const;
    
    int get_relative_anim(int base_seq, int rel_anim) const;    // maps seq local anim reference to global anim index
    int get_relative_seq(int base_seq, int rel_seq) const;      // maps seq local seq reference to global seq index
    
    //private:
    mutable int m_activity_list_version;    // initialization flag - have the sequences been indexed?
    mutable int m_events_indexed;
    
    //public:
    int     get_sequence_activity(int iSequence);
    void    set_sequence_activity(int iSequence, int iActivity);
    int     get_activity_list_version();
    void    set_activity_list_version(int version) const;
    int     get_event_list_version();
    void    set_event_list_version(int version);
    
    // raw textures
    int m_num_textures;
    int m_texture_index;
    
    // raw textures search paths
    int m_num_cd_textures;
    int m_cd_texture_index;
    
    inline char* get_cd_texture(int i) const
    {
        return (((char*)this) + *((int*)(((unsigned char*)this) + m_cd_texture_index) + i));
    };
    
    // replaceable textures tables
    int m_numskinref;
    int m_numskinfamilies;
    int m_skin_index;
    
    inline short* get_skin_ref(int i) const
    {
        return (short*)(((unsigned char*)this) + m_skin_index) + i;
    }
    
    int m_num_body_parts;
    int m_body_part_index;
    
    // queryable attachable points
    //private:
    int m_num_local_attachments;
    int m_local_attachment_index;
    
    //public:
    int get_num_attachments() const;
    
    int get_attachment_bone(int i);
    
    // used on my tools in hlmv, not persistant
    void set_attachment_bone(int attachment, int bone);
    
    // animation node to animation node transition graph
    //private:
    int m_num_local_nodes;
    int m_local_node_index;
    int m_local_node_name_index;
    
    inline char* get_local_node_name(int node) const
    {
        // Assert(iNode >= 0 && iNode < m_num_local_nodes);
        return (((char*)this) + *((int*)(((unsigned char*)this) + m_local_node_name_index) + node));
    }
    
    inline unsigned char* get_local_transition(int i) const
    {
        // Assert(i >= 0 && i < (numlocalnodes*  numlocalnodes));
        return (unsigned char*)(((unsigned char*)this) + m_local_node_index) + i;
    };
    
    //public:
    int     entry_node(int sequence);
    int     exit_node(int sequence);
    char*   get_node_name(int node);
    int     get_transition(int from, int to) const;
    
    int m_num_flex_desc;
    int m_flex_desc_index;
    int m_num_flex_controllers;
    int m_flex_controller_index;
    int m_num_flex_rules;
    int m_flex_rule_index;
    int m_num_ik_chains;
    int m_ik_chain_index;
    int m_num_mouths;
    int m_mouth_index;
    
    //private:
    int m_num_local_pose_parameters;
    int m_local_pose_param_index;
    
    //public:
    int get_num_pose_parameters() const;
    
    int get_shared_pose_parameter(int sequence, int local_pose) const;
    
    int m_surface_prop_index;
    
    inline char* const get_surface_prop() const
    {
        return ((char* ) this) + m_surface_prop_index;
    }
    
    // Key values
    int m_key_value_index;
    int m_key_value_size;
    
    inline const char* get_key_value_text() const
    {
        return m_key_value_size != 0 ? ((char* ) this) + m_key_value_index : NULL;
    }
    
    int m_num_local_ik_autoplay_locks;
    int m_local_ik_autoplay_lock_index;
    
    int get_num__ik_autoplay_locks() const;
    int count_autoplay_sequences() const;
    int copy_autoplay_sequences(unsigned short* out, int count) const;
    int get_autoplay_list(unsigned short** out) const;
    
    // The collision model mass that jay wanted
    float   m_mass;
    int     m_contents;
    
    // external animations, models, etc.
    int     m_num_include_models;
    int     m_include_model_index;
    
    // implementation specific call to get a named model
    const studio_hdr_t* find_model(void** cache, char const* model_name) const;
    
    // implementation specific back pointer to virtual data
    mutable void* m_virtual_model;
    //virtualmodel_t        GetVirtualModel() const;
    
    // for demand loaded animation blocks
    int m_anim_block_name_index;
    
    inline char* const get_anim_block_name() const
    {
        return ((char*)this) + m_anim_block_name_index;
    }
    
    int m_num_anim_blocks;
    int m_anim_block_index;
    mutable void* m_anim_block_model;
    
    unsigned char* get_anim_block(int i) const;
    
    int m_bone_table_by_name_index;
    
    inline const unsigned char* get_bone_table_sorted_by_name() const
    {
        return (unsigned char* ) this + m_bone_table_by_name_index;
    }
    
    // used by tools only that don't cache, but persist mdl's peer data
    // engine uses virtualModel to back link to cache pointers
    void* m_vertex_base;
    void* m_index_base;
    
    // if STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT is set,
    // this value is used to calculate directional components of lighting
    // on static props
    unsigned char m_const_directional_light_dot;
    
    // set during load of mdl data to track* desired* lod configuration (not actual)
    // the* actual* clamped root lod is found in studiohwdata
    // this is stored here as a global store to ensure the staged loading matches the rendering
    unsigned char m_root_lod;
    
    // set in the mdl data to specify that lod configuration should only allow first numAllowRootLODs
    // to be set as root LOD:
    //    numAllowedRootLODs = 0    means no restriction, any lod can be set as root lod.
    //    numAllowedRootLODs = N    means that lod0 - lod(N-1) can be set as root lod, but not lodN or lower.
    unsigned char   m_num_allowed_root_lods;
    unsigned char   m_unused[1];
    int             m_unused4; // zero out if version < 47
    int             m_num_flex_controller_ui;
    int             m_flex_controller_ui_index;
    int             m_unused3[2];
    
    // FIXME: Remove when we up the model version. Move all fields of studiohdr2_t into studiohdr_t.
    int m_studio_hdr2_index;
    
    // NOTE: No room to add stuff? Up the .mdl file format version
    // [and move all fields in studiohdr2_t into studiohdr_t and kill studiohdr2_t],
    // or add your stuff to studiohdr2_t. See NumSrcBoneTransforms/SrcBoneTransform for the pattern to use.
    int m_unused2[1];
    
    studio_hdr_t() { }
    
private:
    
    // No copy constructors allowed
    studio_hdr_t(const studio_hdr_t& other);
    
    friend struct virtual_model_t;
};



class model_info_t
{
public:
    
    model_t* get_model(int index)
    {
        typedef model_t* (*o_get_model)(void*, int);
        return getvfunc<o_get_model>(this, 2)(this, index);
    }
    
    int get_model_index(const char* filename)
    {
        typedef int (*o_get_model_index)(void*, const char*);
        return getvfunc<o_get_model_index>(this, 3)(this, filename);
    }
    
    const char* get_model_name(const model_t* model)
    {
        typedef const char* (*o_get_model_name)(void*, const model_t*);
        return getvfunc<o_get_model_name>(this, 4)(this, model);
    }
    
    void get_model_materials(const model_t* model, int count, material_t** mat)
    {
        typedef studio_hdr_t* (*o_get_model_materials)(void*, const model_t*, int, material_t**);
        getvfunc<o_get_model_materials>(this, 18)(this, model, count, mat);
    }
    
    studio_hdr_t* get_studio_model(const model_t* model)
    {
        typedef studio_hdr_t* (*o_get_studio_model)(void*, const model_t*);
        return getvfunc<o_get_studio_model>(this, 31)(this, model);
    }
};
