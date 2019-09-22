/*
 * util.h
 */
#pragma once

#define pwnedhook(msg) cheat_print(msg, "pwnedhook", color_t(148, 213, 90))

void cheat_print(const std::string& msg, std::string prefix = "cheat", color_t col = color_t::white);

player_info_t util_get_player_info(player_t* player);

bool util_is_point_visible(vec3_t point);
bool util_is_player_visible(player_t* player);
bool util_is_hitbox_visible(player_t* player, int hitbox);

long util_timestamp();

std::string get_weapon_name(weapon_t* weapon);
std::string util_execute_cmd(const std::string& cmd, bool pop = false);
