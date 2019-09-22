/*  key_event.cpp
 *
 *
 */
#include "common.h"

// in_keyevent
int key_event_hook(void* thisptr, int event_code, int key_code, const char* cur_binding)
{
    print_hook();
    
    // pressed
    if(event_code == 0)
    {
        if(key_code == set.menu.key || key_code == KEY_LALT)
        {
            set.menu.open = !set.menu.open;
            
            g_input_system->enable_input(!set.menu.open);
        }
    }
    // released
    else if(event_code == 1)
    {
        
    }
    
    return client_vmt->get_original_method<key_event_fn>(INDEX_KEY_EVENT)(thisptr, event_code, key_code, cur_binding);
}
