/*  g_offsets.cpp
 *
 *
 */
#include "common.h"
#include "offsets.h"
#include "netvar.h"

offsets_t g_offsets;

void offsets_t::init()
{
    string table_name = "";
    
    table_name = "DT_BaseEntity";
    g_offsets.base_entity.m_team        = g_netvar.get_offset(table_name, "m_iTeamNum");
    g_offsets.base_entity.m_collision   = g_netvar.get_offset(table_name, "m_Collision");
    g_offsets.base_entity.m_origin      = g_netvar.get_offset(table_name, "m_vecOrigin");
    g_offsets.base_entity.m_simulation_time = g_netvar.get_offset(table_name, "m_flSimulationTime");
    g_offsets.base_entity.m_move_type   = g_netvar.get_offset(table_name, "m_MoveType");
    
    table_name = "DT_BasePlayer";
    g_offsets.base_player.m_health      = g_netvar.get_offset(table_name, "m_iHealth");
    g_offsets.base_player.m_lifestate   = g_netvar.get_offset(table_name, "m_lifeState");
    g_offsets.base_player.m_view_offset = g_netvar.get_offset(table_name, "m_vecViewOffset");
    g_offsets.base_player.m_tick_base   = g_netvar.get_offset(table_name, "m_nTickBase");
    g_offsets.base_player.m_flags       = g_netvar.get_offset(table_name, "m_fFlags");
    g_offsets.base_player.m_aim_punch_angle = g_netvar.get_offset(table_name, "m_aimPunchAngle");
    g_offsets.base_player.m_view_punch_angle= g_netvar.get_offset(table_name, "m_viewPunchAngle");
    g_offsets.base_player.m_velocity    = g_netvar.get_offset(table_name, "m_vecVelocity");
    
    table_name = "DT_CSPlayer";
    g_offsets.cs_player.m_flash_alpha   = g_netvar.get_offset(table_name, "m_flFlashMaxAlpha");
    g_offsets.cs_player.m_is_scoped     = g_netvar.get_offset(table_name, "m_bIsScoped");
    g_offsets.cs_player.m_lower_body_yaw= g_netvar.get_offset(table_name, "m_flLowerBodyYawTarget");
    g_offsets.cs_player.m_armor         = g_netvar.get_offset(table_name, "m_ArmorValue");
    g_offsets.cs_player.m_has_helmet    = g_netvar.get_offset(table_name, "m_bHasHelmet");
    g_offsets.cs_player.m_immune        = g_netvar.get_offset(table_name, "m_bGunGameImmunity");
            
    table_name = "DT_BaseAttributableItem";
    g_offsets.base_attributable_item.m_item_definition_index = g_netvar.get_offset(table_name, "m_iItemDefinitionIndex");
    
    table_name = "DT_BaseCombatWeapon";
    g_offsets.base_combat_weapon.m_clip1= g_netvar.get_offset(table_name, "m_iClip1");
    g_offsets.base_combat_weapon.m_next_primary_attack = g_netvar.get_offset(table_name, "m_flNextPrimaryAttack");
    
    table_name = "DT_PlantedC4";
    g_offsets.planted_c4.m_bomb_ticking = g_netvar.get_offset(table_name, "m_bBombTicking");
    g_offsets.planted_c4.m_blow_time    = g_netvar.get_offset(table_name, "m_flC4Blow");
    g_offsets.planted_c4.m_bomb_defused = g_netvar.get_offset(table_name, "m_bBombDefused");
    g_offsets.planted_c4.m_bomb_defuser = g_netvar.get_offset(table_name, "m_hBombDefuser");
    g_offsets.planted_c4.m_defuse_count_down = g_netvar.get_offset(table_name, "m_flDefuseCountDown");
    
    table_name = "DT_CSGameRulesProxy";
    
    table_name = "DT_BaseCombatCharacter";
    g_offsets.base_combat_character.m_active_weapon = g_netvar.get_offset(table_name, "m_hActiveWeapon");
}
