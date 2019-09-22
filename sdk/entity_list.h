/*
 *  entity_list.h
 */
#pragma once

#include "base_entity.h"
#include "base_handle.h"
#include "base_player.h"

class entity_list_t
{
public:
    
    base_entity_t* get_entity(int index)
    {
        typedef base_entity_t* (*o_get_entity)(void*, int);
        return getvfunc<o_get_entity>(this, 3)(this, index);
    }
    
    base_player_t* get_player(int index)
    {
        return (base_player_t*)get_entity(index);
    }
    
    base_entity_t* get_entity_from_handle(void* handle)
    {
        typedef base_entity_t* (*o_get_entity_from_handle)(void*, void*);
        return getvfunc<o_get_entity_from_handle>(this, 4)(this, handle);
    }
    
    base_entity_t* get_entity_from_handle(base_handle_t handle)
    {
        typedef base_entity_t* (*o_get_entity_from_handle)(void*, base_handle_t);
        return getvfunc<o_get_entity_from_handle>(this, 4)(this, handle);
    }
    
    int get_highest_index()
    {
        typedef int (*o_get_highest_index)(void*);
        return getvfunc<o_get_highest_index>(this, 6)(this);
    }
};


