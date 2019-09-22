/*
 *  base_attributable_item.h
 */
#pragma once

class base_attributable_item_t : public base_entity_t
{
public:
    
    short* get_item_definition_index_pointer()
    {
        return sdk_get_pointer(short, base_attributable_item, m_item_definition_index);
    }
    
    short get_item_definition_index()
    {
        return *get_item_definition_index_pointer();
    }
    
    bool is_knife()
    {
        switch (get_item_definition_index())
        {
            case WEAPON_KNIFE:
            case WEAPON_KNIFE_T:
            case WEAPON_KNIFE_GUT:
            case WEAPON_KNIFE_FLIP:
            case WEAPON_KNIFE_BAYONET:
            case WEAPON_KNIFE_M9_BAYONET:
            case WEAPON_KNIFE_KARAMBIT:
            case WEAPON_KNIFE_TACTICAL:
            case WEAPON_KNIFE_BUTTERFLY:
            case WEAPON_KNIFE_SURVIVAL_BOWIE:
            case WEAPON_KNIFE_FALCHION:
            case WEAPON_KNIFE_PUSH:
                return true;
            default:
                return false;
        }
    }
    
    bool is_grenade()
    {
        switch (get_item_definition_index())
        {
            case WEAPON_SMOKEGRENADE:
            case WEAPON_HEGRENADE:
            case WEAPON_INCGRENADE:
            case WEAPON_FLASHBANG:
            case WEAPON_MOLOTOV:
            case WEAPON_DECOY:
                return true;
            default:
                return false;
        }
    }
    
    bool is_bomb()
    {
        return get_item_definition_index() == WEAPON_C4;
    }
    
};

typedef base_attributable_item_t attributable_item_t;
