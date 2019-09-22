/*
 *  util_math.h
 */
#pragma once

bool world_to_screen(vec3_t& in, vec3_t& out);
player_box_t get_player_box(base_entity_t* entity);

float util_curtime_fixed();

void angle_vectors(const qangle_t& angles, vec3_t& forward);
void angle_vectors(const qangle_t& angles, vec3_t& forward, vec3_t& right, vec3_t& up);
void vector_angles(const vec3_t& forward, qangle_t& angles);

void normalise_angles(qangle_t& angle);

void vector_transform(vec3_t& in1, matrix3x4_t& in2, vec3_t& out);

float get_fov(const qangle_t& view, const qangle_t& aim);

qangle_t calculate_angle(const vec3_t& src, const vec3_t& dst);

vec3_t get_hitbox_position(player_t* player, int hitbox);


template<class T>
T util_clamp(T in, T min, T max)
{
    if(in < min)
        return min;
    if(in > max)
        return max;
    return in;
}
