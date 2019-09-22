/*  override_view.cpp
 *
 *
 */
#include "common.h"

void override_view_hook(void* thisptr, view_setup_t* setup)
{
    if(set.misc.fov && global::local && !global::local->is_scoped())
    {
        float fov = setup->m_fov;
        fov += set.misc.fov;
        setup->m_fov = fov;
    }
    
    if(set.misc.thirdperson)
    {
        // todo
    }
    
    client_mode_vmt->get_original_method<override_view_fn>(INDEX_OVERRIDE_VIEW)(thisptr, setup);
}
