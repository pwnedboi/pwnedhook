/*      util_math.cpp
 *
 *
 */
#include "common.h"

/*
 *  world_to_screen
 *  Converts a point in the game to a point on the screen
 */
bool world_to_screen(vec3_t& in, vec3_t& out)
{
    return !g_overlay->screen_position(in, out);
}

/*
 *  get_player_box
 *  Returns the bounds of the player as a box
 */
player_box_t get_player_box(base_entity_t* entity)
{
    player_box_t box;
    
    if (!entity)
        return box;
    
    vec3_t origin = entity->get_origin();
    vec3_t min = entity->get_collidable()->obb_min() + origin;
    vec3_t max = entity->get_collidable()->obb_max() + origin;
    
    vec3_t points[] =
    {
        vec3_t(min.x, min.y, min.z),
        vec3_t(min.x, max.y, min.z),
        vec3_t(max.x, max.y, min.z),
        vec3_t(max.x, min.y, min.z),
        vec3_t(max.x, max.y, max.z),
        vec3_t(min.x, max.y, max.z),
        vec3_t(min.x, min.y, max.z),
        vec3_t(max.x, min.y, max.z)
    };
    
    vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
    
    if (!world_to_screen(points[0], blb) || !world_to_screen(points[1], brb) || !world_to_screen(points[2], frb) ||
        !world_to_screen(points[3], flb) || !world_to_screen(points[4], frt) || !world_to_screen(points[5], brt) ||
        !world_to_screen(points[6], blt) || !world_to_screen(points[7], flt))
        return box;
    
    vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
    
    float left   = flb.x;   // left
    float top    = flb.y;   // top
    float right  = flb.x;   // right
    float bottom = flb.y;   // bottom
    
    for (int i = 1; i < 8; i++)
    {
        if (left > arr[i].x)
            left = arr[i].x;
        if (bottom < arr[i].y)
            bottom = arr[i].y;
        if (right < arr[i].x)
            right = arr[i].x;
        if (top > arr[i].y)
            top = arr[i].y;
    }
    
    box.x = left;
    box.y = top;
    box.w = right - left;
    box.h = bottom - top;
    box.valid = true;
}

/*
 *
 *
 */
float util_curtime_fixed()
{
    if (!global::local)
        return 0;
    
    static int tick = 0;
    static user_cmd_t* last_cmd = nullptr;
    
    if (!last_cmd || last_cmd->m_has_been_predicted)
        tick = (float)global::local->get_tick_base();
    else
        ++tick;

    last_cmd = global::cmd;
    return tick * g_globals->m_interval_per_tick;
}

/*
 *  sin_cos
 */
static void inline sin_cos(float radians, float *sine, float *cosine)
{
    double __cosr, __sinr;
    __asm ("fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (radians));
    
    *sine = __sinr;
    *cosine = __cosr;
}

/*
 *
 *
 */
void angle_vectors(const qangle_t& angles, vec3_t& forward)
{
    float sp, sy, cp, cy;
    
    sin_cos(DEG2RAD(angles[YAW]), &sy, &cy);
    sin_cos(DEG2RAD(angles[PITCH]), &sp, &cp);
    
    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;
}

/*
 *
 *
 */
void angle_vectors(const qangle_t& angles, vec3_t& forward, vec3_t& right, vec3_t& up)
{
    float sr, sp, sy, cr, cp, cy;
    
    sin_cos(DEG2RAD(angles[1]), &sy, &cy);
    sin_cos(DEG2RAD(angles[0]), &sp, &cp);
    sin_cos(DEG2RAD(angles[2]), &sr, &cr);
    
    forward.x = (cp * cy);
    forward.y = (cp * sy);
    forward.z = (-sp);
    right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
    right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
    right.z = (-1 * sr * cp);
    up.x = (cr * sp * cy + -sr * -sy);
    up.y = (cr * sp * sy + -sr * cy);
    up.z = (cr * cp);
}

/*
 *
 *
 */
void vector_angles(const vec3_t& forward, qangle_t& angles)
{
    if (forward[1] == 0.0f && forward[0] == 0.0f)
    {
        angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
        angles[1] = 0.0f;  //yaw left/right
    }
    else
    {
        angles[0] = atan2(-forward[2], forward.length_2d()) * -180 / M_PI;
        angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;
        
        if (angles[1] > 90)
            angles[1] -= 180;
        else if (angles[1] < 90)
            angles[1] += 180;
        else if (angles[1] == 90)
            angles[1] = 0;
    }
    
    angles[2] = 0.0f;
}

void normalise_angles(qangle_t& angle)
{
    while (angle.x > 89.0f)
        angle.x -= 180.f;
    
    while (angle.x < -89.0f)
        angle.x += 180.f;
    
    while (angle.y > 180.f)
        angle.y -= 360.f;
    
    while (angle.y < -180.f)
        angle.y += 360.f;
}

/*
 *
 *  Returns the fov to from x to y
 */
float get_fov(const qangle_t& view, const qangle_t& aim)
{
    qangle_t delta = aim - view;
    normalise_angles(delta);
    return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

/*
 *
 *  Calculates the angle between 2 vectors
 */
qangle_t calculate_angle(const vec3_t& src, const vec3_t& dst)
{
    qangle_t angles;
    vec3_t delta = src - dst;
    
    vector_angles(delta, angles);
    
    delta.normalise();
    
    return angles;
}

/*
 *
 *
 */
void vector_transform(vec3_t& in1, matrix3x4_t& in2, vec3_t& out)
{
    out.x = _vec_dot_product(in1, vec3_t(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
    out.y = _vec_dot_product(in1, vec3_t(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
    out.z = _vec_dot_product(in1, vec3_t(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}

/*
 *
 *
 */
vec3_t get_hitbox_position(player_t* player, int hitbox)
{
    if(!player)
        return vec3_t(0, 0, 0);
    
    matrix3x4_t matrix[128];
    
    if (!player->setup_bones(matrix, 128, 0x100, g_globals->m_cur_time))
        return vec3_t(0, 0, 0);
    
    studio_hdr_t* hdr = g_model_info->get_studio_model(player->get_model());
    
    if(!hdr)
        return vec3_t(0, 0, 0);
    
    studio_hitbox_set_t* set = hdr->get_hitbox_set(0);
    
    if(!set)
        return vec3_t(0, 0, 0);
    
    studio_bbox_t* bbox = set->get_hitbox(hitbox);
    
    if (!bbox)
        return vec3_t(0, 0, 0);
    
    vec3_t min, max, center;
    vec3_t bbmin = bbox->m_bbmin;
    vec3_t bbmax = bbox->m_bbmax;
    int    bone  = bbox->m_bone;
    
    if(bone >= 128 || bone < 0)
        return vec3_t(0, 0, 0);
    
    matrix3x4_t m = matrix[bone];
    
    vector_transform(bbmin, m, min);
    vector_transform(bbmax, m, max);
    
    bbox->m_bbmin = bbmin;
    bbox->m_bbmax = bbmax;
    matrix[bone]  = m;
    
    center = (min + max) * 0.5f;

    /*
    int pointscale = 50;
    float ptwoz = 50 - pointscale;
    float zval  = (ptwoz * min.z + pointscale * max.z) / 50;
    center.z = zval;
     */
    
    return center;
}
