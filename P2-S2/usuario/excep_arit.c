/*
 * usuario/excep_arit.c
 *
 *  Minikernel. Versi�n 1.0
 *
 *  Fernando P�rez Costoya
 *
 */

/*
 * Programa de usuario que fuerza una excepci�n aritm�tica
 */

#include "servicios.h"

#define TOT_ITER 200    /* n�mero total de iteraciones */
#define ITER_EXCEP 5   /* iteraci�n en la que causa excepci�n */

int tot=0;

int main(){
    printf("PID: %d\n", srv_get_current_process_id());
    int i;
    int tot=0;

    int k;
    printf("Snooooze...\n");
    k = srv_sleep(5);
    printf("Waking up [%d]...\n", k);

    for (i=0; i<TOT_ITER; i++){
        printf("excep_arit: i %d \n", i);

        /* Forzando una excepci�n */
        if ((i+1)%ITER_EXCEP==0)
            i/=tot;
    }
    /* No deber�a llegar ya que ha generado una excepci�n */
    printf("excep_arit: termina\n");
    return 0;
}
