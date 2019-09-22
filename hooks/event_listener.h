/*
 *  event_listener.h
 */
#pragma once

#include "common.h"
#include "events.h"

class event_listner_t : public game_event_listener2_t
{
public:
    
    event_listner_t()
    {
        
    }
    
    ~event_listner_t()
    {
         g_game_events->remove_listener(this);
    }
    
    void add_listner(std::string event_name)
    {
        g_game_events->add_listener(this, event_name.c_str(), false);
    }
    
    void add_listeners(std::vector<std::string> event_names)
    {
        for(auto e : event_names)
            g_game_events->add_listener(this, e.c_str(), false);
    }
    
    void fire_game_event(game_event_t* event) override
    {
        // not in game so we dont need to parse events
        if(!g_engine->is_in_game())
            return;
        
        // pass the event to our functions
        bullet_impact(event);
        player_hurt(event);
    }
    
    int  get_event_debug_id() override
    {
        return EVENT_DEBUG_ID_INIT;
    }
};
