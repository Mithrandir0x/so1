/**
 * usuario/longsum_prio.c
 *
 * A little program that counts sand...
 * But halfway, it just think it's not so important
 * to think about counting sand, so it tells the kernel
 * to reprioritize itself.
 */

#include "servicios.h"
#include <stdio.h>

#define COUNT   4294967295L
#define TRIGGER 2147483646L

int main(){
    unsigned long i = COUNT;
    printf("longsum_prio: PID %d\n", srv_get_current_process_id());

    srv_set_priority(40);

    srv_sleep(1);

    printf("longsum_prio: Beginning to count sand...\n");
    while ( --i ) {
        if ( i == TRIGGER )
        {
            printf("longsum_prio: Setting priority to minimal\n");
            srv_set_priority(10);
        }
    };

    printf("longsum_prio: finishes\n");

    return 0;
}
