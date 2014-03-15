/*
 * usuario/dormilon.c
 *
 */

#include "servicios.h"

int main()
{
    printf("PID: %d\n", srv_get_current_process_id());
    printf("Snoooorlaxing...\n");
    
    srv_sleep(3);
    
    printf("WHAAAAAAGHH!! Raging up!!\n");


    printf("Snoooorlaxing again...\n");
    
    srv_sleep(1);
    
    printf("WHAAAAAAGHH!! Raging up again!!\n");

    printf("dormilon: termina\n");
    
    return 0;
}
