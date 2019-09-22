/*  events.cpp
 *
 *
 */
#include "common.h"
#include "events.h"

void bullet_impact(game_event_t* event)
{
    if(strcmp(event->get_name(), __FUNCTION__))
        return;
    
}

void player_hurt(game_event_t* event)
{
    if(strcmp(event->get_name(), __FUNCTION__))
        return;
    
    hitmarker_event_player_hurt(event);
}
