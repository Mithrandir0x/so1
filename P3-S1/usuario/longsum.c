/**
 * usuario/longsum.c
 *
 * A little program that counts sand...
 */

#include "servicios.h"
#include <stdio.h>

int main(){
    unsigned long i = 4294967295L;
    printf("longsum: PID %d\n", srv_get_current_process_id());

    srv_set_priority(30);

    srv_sleep(1);

    printf("longsum: Beginning to count sand...\n");
    while ( --i );

    printf("longsum: finishes\n");

    return 0;
}
