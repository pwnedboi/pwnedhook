/*  lock_cursor.cpp
 *
 *
 */
#include "common.h"

void lock_cursor_hook(void* thisptr)
{
    print_hook();
    
    if(set.menu.open)
    {
        if(g_surface)
            g_surface->unlock_cursor();
        
        return;
    }
    
    surface_vmt->get_original_method<lock_cursor_fn>(INDEX_LOCK_CURSOR)(thisptr);
}
