/*
 * usuario/init.c
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

#include "servicios.h"

int main()
{
    printf("PID: %d\n", srv_get_current_process_id());
    printf("init: comienza\n");

    // Has priority 20 and does not sleep
    //crear_proceso("longsum");

    // Has priority 40, but sleeps for 2 seconds
    crear_proceso("simplon_prio");

    // Has priority 17, but sleeps for 10 seconds
    crear_proceso("dormilon");

    // Has priority 13 and does not sleep
    crear_proceso("getc");

    printf("init: termina\n");
    
    return 0; 
}
