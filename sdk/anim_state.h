/*
 *  anim_state.h
 */
#pragma once

class anim_layer_t
{
public:
    char    _pad0[24];
    int     m_order;
    int     m_sequence;
    float   m_prev_cycle;
    float   m_weight;
    float   m_weight_delta_rate;
    float   m_playback_rate;
    float   m_cycle;
    void*   m_owner;        // players thisptr
    char    _pad1[8];       // 0x40
}; //Size: 0x0048

class anim_state_t
{
public:
    char    pad_0008[128];              //0x0000
    float   last_anim_update_time;      //0x0080
    int     last_anim_update_frame;     //0x0084
    float   eye_pitch;                  //0x0088
    float   yaw;                        //0x008C
    float   pitch;                      //0x0090
    float   goal_feet_yaw;              //0x0094
    float   current_feet_yaw;           //0x0098
    float   abs_movement_direction;     //0x009C Movement direction on a compass, North = 0, East = 90, South = 180...
    float   last_abs_movement_direction;//0x00A0 ^^^^^ for last tick
    float   lean_amount;                //0x00A4
    char    pad_00A8[4];                //0x00A8
    float   feet_cycle;                 //0x00AC Progress from 0.0-1.0 of the legs moving animation.
    float   feet_yaw_rate;              //0x00B0 How fast to play feetCycle ( walking = slower )
    char    pad_00B4[4];                //0x00B4
    float   duck_progress;//0x00B8 Progress 0.0-1.0, 0.0 = standing, 1.0 = fully crouched. Also used for jumping. Counts backwards from 1.0 for jumps
    float   landing_animation_time_left_until_done; //0x00BC Adds some time when landing animation starts.
    char    pad_00C0[4];                //0x00C0
    vec3_t  origin;                     //0x00C4
    vec3_t  last_origin;                //0x00D0
    float   velocity_x;                 //0x00DC Speeds on axis
    float   velocity_y;                 //0x00E0 ^^^^^^^^^^
    char    pad_00E4[4];                //0x00E4
    float   current_direction_x;        //0x00E8 -1.0 to 1.0. West is 1.0, East is -1.0. North and South are both 0
    float   current_direction_y;        //0x00EC -1.0 to 1.0. North is -1.0, South is 1.0. East and West are both 0
    char    pad_00F0[4];                //0x00F0
    float   last_known_direction_x;     //0x00F4 Same as its counterpart above, but this is saved until the next update
    float   last_known_direction_y;     //0x00F8 ^^^^^^^^^^^^^^^^^^^^^^^
    char    pad_00FC[4];                //0x00FC
    float 	speed_2d;                   //0x0100 Affected by slopes. ( got 240 with knife on a hill )   // horizontal_velocity
    float 	vertical_velocity;          //0x0104. Goes up when jumping, negative when falling           // up velocity
    float 	speed;                      //0x0108 0.0 -> 1.0, how fast you are going; 1.0 = full speed.
    float 	feet_shuffle_speed;         //0x010C // 0.0-2.0 (approx)
    float   feet_shuffle_Speed2;        //0x0110 // 0.0-3.0 (approx)
    float   time_since_started_moving;  //0x0114 Jumping and Crouching do not affect these
    float   time_since_stopped_moving;  //0x0118 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    bool    on_ground;                  //0x011C True if on ground (do I need to type this?)
    bool    in_jump_recovery_anim;      //0x011D True if you're doing one of the retarded knee buckling animations
    char    pad_011E[10];               //0x011E
    float   height_before_jump;         //0x0128 Updated right when you jump
    char    pad_012C[4];                //0x012C
    float   running_accel_progress;     //0x0130 0 - 1.0, Only affected by Running, reaches 1.0 when you are at full speed (ex: 250 w/ knife )
    char    pad_0134[68];               //0x0134
    char 	unkown[572];                // Overall Size should be 0x3B0(+4), padding the end here.
}; // size: 0x0178
