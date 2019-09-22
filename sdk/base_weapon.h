/*
 * base_weapon.h
 */
#pragma once

class base_weapon_t : public base_attributable_item_t
{
public:
    
    bool is_weapon()
    {
        // todo
        return true;
    }
    
    int get_ammo()
    {
        return sdk_get_offset(int, base_combat_weapon, m_clip1);
    }
    
    float get_next_attack()
    {
        return sdk_get_offset(float, base_combat_weapon, m_next_primary_attack);
    }
    
    weapon_info_t* get_weapon_info()
    {
        // todo : get the right index
        // 516, 522
        typedef weapon_info_t*(*o_get_weapon_info)(void*);
        return getvfunc<o_get_weapon_info>(this, 516)(this);
    }
    
    /*
    void DrawCrosshair() { // xref: CHudCrosshair
        typedef void (* oDrawCrosshair)( void* );
        return getvfunc<oDrawCrosshair>( this, 466 )( this );
    }
    
    CCSWeaponInfo* GetCSWpnData() { // "script file not found"
        typedef CCSWeaponInfo* (* oGetCSWpnData)( void* );
        return getvfunc<oGetCSWpnData>( this, 522 )( this ); // 516
    }
    
    float GetSpread() {
        typedef float (* oGetSpread)( void* );
        return getvfunc<oGetSpread>( this, 514 )( this );
    }
    
    float GetInaccuracy() {
        typedef float (* oGetInaccuracy)( void* );
        return getvfunc<oGetInaccuracy>( this, 544 )( this );
    }
    
    void UpdateAccuracyPenalty() {
        typedef void (* oUpdateAccuracyPenalty)( void* );
        return getvfunc<oUpdateAccuracyPenalty>( this, 545 )( this );
    }
    */
};

typedef base_weapon_t weapon_t;
