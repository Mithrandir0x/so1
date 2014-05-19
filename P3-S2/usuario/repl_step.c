/**
 * usuario/repl_step.c
 *
 * This program forks itself until MAX_DEPTH. Each child takes
 * a bit longer to finish.
 *
 * It allows to test if child process reference to the correct
 * parent process.
 */

#include "servicios.h"

// How many child processes will be created
#define MAX_DEPTH 5

int main()
{
    static int depth = 0;
    int current_depth;
    unsigned long long i = 134217727L;
    
    depth++;
    current_depth = depth;

    printf("repl_step[%d]: PID [%d]\n", current_depth, srv_get_current_process_id());

    i = depth * i;
    printf("repl_step[%d]: depth = [%d]\n", current_depth, depth);

    if ( depth < MAX_DEPTH )
        crear_proceso("repl_step");

    printf("repl_step[%d]: Beginning to count until [%llu]...\n", current_depth, i);
    while ( --i );

    printf("repl_step[%d]: waiting for child to end\n", current_depth);

    srv_wait();

    printf("repl_step[%d]: finishes\n", current_depth);

    return 0;
}
