/*
 *  minikernel/include/kernel.h
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Fichero de cabecera que contiene definiciones usadas por kernel.c
 */

#ifndef _KERNEL_H
#define _KERNEL_H

#include "const.h"
#include "HAL.h"
#include "llamsis.h"

/*
 * Definicion del tipo que corresponde con el BCP.
 * Se va a modificar al incluir la funcionalidad pedida.
 */
typedef struct BCP_t *BCPptr;

typedef struct BCP_t {
    int id;                     /* ident. del proceso */
    int estado;                 /* TERMINADO|LISTO|EJECUCION|BLOQUEADO*/
    contexto_t contexto_regs;   /* copia de regs. de UCP */
    void * pila;                /* dir. inicial de la pila */
    BCPptr siguiente;           /* puntero a otro BCP */
    void *info_mem;             /* descriptor del mapa de memoria */
    unsigned int tts;           /* Ticks-To-Sleep (ticks to be sleeping before waking up) */
    unsigned int priority;      /* integer value of priority, the more it has, the more important is */
} BCP;

/*
 * Definicion del tipo que corresponde con la cabecera de una lista
 * de BCPs. Este tipo se puede usar para diversas listas (procesos listos,
 * procesos bloqueados en semáforo, etc.).
 */
typedef struct{
    BCP *primero;
    BCP *ultimo;
} lista_BCPs;


/*
 * Variable global que identifica el proceso actual
 */

BCP *p_proc_actual = NULL;

/*
 * Variable global que representa la tabla de procesos
 */

BCP tabla_procs[MAX_PROC];

/*
 * Variable global que representa la cola de procesos listos
 */
lista_BCPs lista_listos = {NULL, NULL};

lista_BCPs l_slept_procs = { NULL, NULL };

int f_pending_planning = NULL;

/*
 * Definición del tipo que corresponde con una entrada en la tabla de
 * llamadas al sistema.
 */
typedef struct {
    int (*fservicio)();
} servicio;

/*
 * Prototipos de las rutinas que realizan cada llamada al sistema
 */
int sis_crear_proceso();
int sis_terminar_proceso();
int sis_escribir();
int sys_get_current_pid();
int sys_sleep();
int sys_set_priority();

/*
 * Variable global que contiene las rutinas que realizan cada llamada
 */
servicio tabla_servicios[NSERVICIOS] = {
    {sis_crear_proceso},
    {sis_terminar_proceso},
    {sis_escribir},
    {sys_get_current_pid},
    {sys_sleep},
    {sys_set_priority}
};

/**
 * Coment out this line to enable debug output when updating BCP's TTS's.
 */
//#define __KRN_DBG_UPDATE_SLEPT_PROCESS__

/**
 * WARNING! ACHTUNG! ATENCIO! ATENCION!
 *
 *    UNCOMMENT ONLY 1 SCHEDULER
 *          AT A TIME!!!
 *
 * WARNING! ACHTUNG! ATENCIO! ATENCION!
 *
 * __KRN_SCHEDULER_DEFAULT__
 *   This is the first scheduler. When the scheduler is called,
 *   it returns the next task available in the ready BCP list.
 *
 * __KRN_SCHEDULER_PRIORITIES__
 *   This scheduler returns the 
 */
#define __KRN_SCHEDULER_DEFAULT__
//#define __KRN_SCHEDULER_PRIORITIES__

#endif /* _KERNEL_H */
