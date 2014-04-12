/*
 * usuario/dormilon.c
 *
 */

#include "servicios.h"

int main()
{
    printf("dormilon: PID %d\n", srv_get_current_process_id());
    printf("dormilon: Snoooorlaxing...\n");

    srv_set_priority(17);
    
    srv_sleep(10);
    
    printf("dormilon: WHAAAAAAGHH!! Raging up!!\n");

    printf("dormilon: termina\n");
    
    return 0;
}
