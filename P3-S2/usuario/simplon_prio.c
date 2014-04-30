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
    
    printf("simplon_prio: PID: %d\n", srv_get_current_process_id());

    printf("simplon_prio: Prioritizing...\n");
    srv_set_priority(40);

    printf("simplon_prio: Snooooze...\n");
    k = srv_sleep(2);
    printf("simplon_prio: Waking up [%d]...\n", k);

    for (i=0; i < EASY_ITERATIONS; i++)
        printf("simplon_prio: i %d\n", i);

    printf("simplon_prio: termina\n");
    return 0;
}
