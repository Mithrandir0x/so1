/*
 * usuario/init.c
 *
 *  Minikernel. Versi�n 1.0
 *
 *  Fernando P�rez Costoya
 *
 */

#include "servicios.h"

typedef void (*TEST_F)(void);

/*
 * So what does this test do? Given all those process, we want to see
 * if priority is being shared accordingly to what the assignment asks
 * for.
 *
 * Though it is possible to make "getc" end the first one, the idea of
 * using this program is that we see how it interacts with "longsum",
 * as both process are sharing CPU time.
 *
 * Also it is important to see when process awake, they can have a
 * greater effective priority and shove off the current process being
 * executed.
 */
static void test_0()
{
    printf("init: ejecutando test_0\n");

    // Has priority 30 and does not sleep
    crear_proceso("longsum");

    // Has priority 40, but sleeps for 2 seconds
    crear_proceso("simplon_prio");

    // Has priority 17, but sleeps for 10 seconds
    crear_proceso("dormilon");

    // Has priority 13 and does not sleep
    crear_proceso("getc");
}

/*
 * And this one? Both process do the same, they start counting a little bag
 * of sand, but "longsum_prio" at halfway changes the priority, making it
 * to count slower that "longsum" as it would have lower CPU time.
 *
 * Nevertheless, I should give it a cookie for the effort.
 */
static void test_1()
{
    printf("init: ejecutando test_1\n");

    // Has priority 30, and does not change throughout all its execution.
    crear_proceso("longsum");

    // Has priority 20, but changes to 10 at some moment.
    crear_proceso("longsum_prio");
}

static void test_2()
{
    crear_proceso("replicate");
}

int main()
{
    TEST_F tests[3] = { &test_0, &test_1, &test_2 };
    
    TEST_F test = tests[2];
    
    printf("PID: %d\n", srv_get_current_process_id());
    printf("init: comienza\n");

    printf("PPID: %d\n", srv_get_parent_pid());

    test();

    printf("init: termina\n");
    
    return 0; 
}
