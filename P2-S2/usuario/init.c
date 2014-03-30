/*
 * usuario/init.c
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

/* Programa inicial que arranca el S.O. Sólo se ejecutarán los programas
   que se incluyan aquí, que, evidentemente, pueden ejecutar otros
   programas...
*/

#include "servicios.h"

int main()
{
    printf("PID: %d\n", srv_get_current_process_id());
    printf("init: comienza\n");

    crear_proceso("longsum");

    crear_proceso("simplon_prio");

    crear_proceso("dormilon");

    crear_proceso("getc");

    /* Este programa causa una excepción */
    //crear_proceso("excep_arit");

    /* Este programa crea otro proceso que ejecuta simplon a una excepción */
    //crear_proceso("excep_mem");
    
    /* No existe: debe fallar */
    //crear_proceso("noexiste");   

    printf("init: termina\n");
    
    return 0; 
}
