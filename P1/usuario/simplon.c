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
    printf("PID: %d\n", srv_get_current_process_id());
    int i;

    int k;
    printf("Snooooze...\n");
    k = srv_sleep(5);
    printf("Waking up [%d]...\n", k);

    for (i=0; i<TOT_ITER; i++)
        printf("simplon: i %d\n", i);

    printf("simplon: termina\n");
    return 0;
}
