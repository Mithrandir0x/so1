/*
 * usuario/init.c
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

/* Programa inicial que arranca el S.O. Sólo se ejecutarán los programas
   que se incluyan aquí, que, evidentemente, pueden ejecutar otros
   programas...
*/

#include "servicios.h"

#define TEST_1 1
#define TEST_2 2

int main()
{
    int test = TEST_1;

    printf("PID: %d\n", srv_get_current_process_id());
    printf("init: comienza\n");

    /*
     * So what does this test do? Given all those process, changing the priority
     * does make the CPU to change from a process to another, based on the
     * priority shift.
     */
    if ( test == TEST_1 )
    {
        printf("init: ejecutando TEST_1\n");
        
        // Has priority 20
        crear_proceso("longsum");

        // Has priority 35
        crear_proceso("simplon_prio");

        // Has default priority (i.e. 10)
        crear_proceso("getc");
    }

    /*
     * And this one? Both process do the same, they start counting a little bag
     * of sand, but "longsum_prio" at halfway changes the priority, making it
     * to count slower that "longsum" as it would have lower CPU time.
     *
     * Nevertheless, I should give it a cookie for the effort.
     */
    if ( test == TEST_2 )
    {
        printf("init: ejecutando TEST_2\n");

        // Has priority 30, and does not change throughout all its execution.
        crear_proceso("longsum");

        // Has priority 40, but changes to 10 at some moment.
        crear_proceso("longsum_prio");
    }

    printf("init: termina\n");
    
    return 0; 
}
