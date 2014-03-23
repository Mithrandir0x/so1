/*
 * usuario/init.c
 *
 *  Minikernel. Versi�n 1.0
 *
 *  Fernando P�rez Costoya
 *
 */

/* Programa inicial que arranca el S.O. S�lo se ejecutar�n los programas
   que se incluyan aqu�, que, evidentemente, pueden ejecutar otros
   programas...
*/

#include "servicios.h"

int main()
{
    printf("PID: %d\n", srv_get_current_process_id());
    printf("init: comienza\n");

    crear_proceso("simplon_prio");

    /* Este programa causa una excepci�n */
    //crear_proceso("excep_arit");

    /* Este programa crea otro proceso que ejecuta simplon a una excepci�n */
    //crear_proceso("excep_mem");
    
    /* No existe: debe fallar */
    //crear_proceso("noexiste");

    /* Snorlax goes to sleep... */
    //crear_proceso("dormilon");

    printf("init: termina\n");
    
    return 0; 
}
