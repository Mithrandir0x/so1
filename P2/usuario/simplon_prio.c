/**
 * usuario/simplon_prio.c
 *
 */

#include "servicios.h"

#define EASY_ITERATIONS 5

int main()
{
    int i;
    int k;
    
    printf("PID: %d\n", srv_get_current_process_id());

    printf("Prioritizing...\n");
    srv_set_priority(34);

    printf("Snooooze...\n");
    k = srv_sleep(1);
    printf("Waking up [%d]...\n", k);

    for (i=0; i < EASY_ITERATIONS; i++)
        printf("simplon_prio: i %d\n", i);

    printf("simplon_prio: termina\n");
    return 0;
}
