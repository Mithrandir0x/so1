
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

    printf("replicate[%d]: PID [%d]\n", current_depth, srv_get_current_process_id());

    i = depth * i;
    printf("replicate[%d]: depth = [%d]\n", current_depth, depth);

    if ( depth < MAX_DEPTH )
        crear_proceso("replicate");

    printf("replicate[%d]: Beginning to count until [%llu]...\n", current_depth, i);
    while ( --i );

    printf("replicate[%d]: finishes\n", current_depth);

    return 0;
}
