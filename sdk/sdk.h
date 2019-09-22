/*
 *  sdk.h
 */
#pragma once

// todo : use the full base_x_t instead of just x_t (player_t, entity_t) in the sdk
// todo : use std:: for everything in the sdk
// todo : clean up

// vec
#include "vec3.h"
#include "vec2.h"
#include "qangle.h"
#include "matrix.h"

// other
#include "definitions.h"
#include "enums.h"
#include "structs.h"
#include "vmt.h"
#include "interface.h"

// memory
#include "netvar.h"
#include "offsets.h"

// csgo
#include "color.h"
#include "cutlmemory.h"
#include "app_system.h"
#include "panel.h"
#include "surface.h"
#include "base_client_dll.h"
#include "cvar.h"
#include "engine.h"
#include "entity_list.h"
#include "debug_overlay.h"
#include "global_vars.h"
#include "engine_trace.h"
#include "client_mode.h"
#include "input_system.h"
#include "model_render.h"
#include "model_info.h"
#include "material_system.h"
#include "client_state.h"
#include "input.h"
#include "physics.h"
// todo
#include "render_view.h"
#include "sdl_manager.h"
#include "game_rules.h"
#include "glow_manager.h"
// todo : interfaces
#include "view_setup.h"
#include "user_cmd.h"
#include "material.h"
#include "anim_state.h"
#include "base_handle.h"
#include "base_entity.h"
#include "base_player.h"
#include "base_attributable_item.h"
#include "base_weapon.h"
#include "base_planted_c4.h"
#include "net_channel.h"
#include "game_event.h"
// todo : game classes
