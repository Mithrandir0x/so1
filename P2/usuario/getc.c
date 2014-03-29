/**
 * usuario/getc.c
 *
 * This little program just waits for a key to be pushed.
 *
 * So, you know...
 * Be pushy...
 * Use the push...
 * Do a barrel push...
 */

#include "servicios.h"
#include <stdio.h>

int main(){
    printf("getc: PID [%d]\n", srv_get_current_process_id());

    printf("getc: Waiting for input...\n");
    getchar();

    printf("getc: Finishes\n");
    return 0;
}
