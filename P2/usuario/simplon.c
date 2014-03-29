/*
 * usuario/simplon.c
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Programa de usuario que simplemente imprime un valor entero
 */

#include "servicios.h"

#define TOT_ITER 5 /* ponga las que considere oportuno */

int main(){
    int pid = srv_get_current_process_id();
    int i;
    
    printf("simplon: PID [%d]\n", pid);
    
    printf("simplon: Snooooze...\n");
    srv_sleep(1);
    printf("simplon: Waking up...\n");

    for (i=0; i<TOT_ITER; i++)
        printf("simplon: i %d\n", i);

    printf("simplon: termina\n");
    return 0;
}
