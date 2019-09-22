/*
 *  pwnedhook v2
 *
 *  credit to w3smac on uc for memory functions
 */

#include "common/hooker.h"

/*
 *  entry point
 */
int __attribute((constructor)) entry()
{
    cheat_init();
    
    cheat_start();
    
    return 0;
}

