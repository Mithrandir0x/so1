/*
 *  usuario/lib/serv.c
 *
 *  Minikernel. Versi�n 1.0
 *
 *  Fernando P�rez Costoya
 *
 */

/*
 *
 * Fichero que contiene las definiciones de las funciones de interfaz
 * a las llamadas al sistema. Usa la funcion de apoyo llamsis
 *
 *      SE DEBE MODIFICAR AL INCLUIR NUEVAS LLAMADAS
 *
 */

#include "llamsis.h"
#include "servicios.h"

/* Funci�n del m�dulo "misc" que prepara el c�digo de la llamada
   (en el registro 0), los par�metros (en registros 1, 2, ...), realiza la
   instruccion de llamada al sistema  y devuelve el resultado 
   (que obtiene del registro 0) */

int llamsis(int llamada, int nargs, ... /* args */);


/*
 *
 * Funciones interfaz a las llamadas al sistema
 *
 */


int crear_proceso(char *prog){
    return llamsis(CREAR_PROCESO, 1, (long)prog);
}
int terminar_proceso(){
    return llamsis(TERMINAR_PROCESO, 0);
}
int escribir(char *texto, unsigned int longi){
    return llamsis(ESCRIBIR, 2, (long)texto, (long)longi);
}

int srv_get_current_process_id()
{
    return llamsis(SYS_ID_GET_PID, 0);
}

int srv_sleep(unsigned int seconds)
{
    return llamsis(SYS_ID_SLEEP, 1, (long) seconds);
}

int srv_set_priority(unsigned int priority)
{
    return llamsis(SYS_GET_PRIORITY, 1, (long) priority);
}

int srv_get_parent_pid()
{
    return llamsis(SYS_GET_PARENT_PID, 0);
}

int srv_wait()
{
    return llamsis(SYS_WAIT, 0);
}
