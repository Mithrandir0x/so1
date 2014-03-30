/*
 *  minikernel/include/const.h
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 *
 * Fichero de cabecera que contiene definiciones de constantes
 *
 *
 */

#ifndef _CONST_H
#define _CONST_H

#ifndef NULL
#define NULL (void *) 0     /* por si acaso no esta ya definida */
#endif

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define true  1
#define false 0

#define MAX_PROC 10     /* dimension de tabla de procesos */

#define TAM_PILA 32768


/*
 * Posibles estados del proceso
 */
#define NO_USADA 0      /* Entrada de tabla de procs. no usada */
#define TERMINADO 0     /* Proc. TERMINADO == entrada NO_USADA */
#define LISTO 1
#define EJECUCION 2
#define BLOQUEADO 3

/*
 * Niveles de ejecución del procesador. 
 */
#define NUM_NIVELES 3
#define NIVEL_0 0 /* User mode */
#define NIVEL_1 1 /* Int. SW */
#define NIVEL_2 2 /* Int. Terminal */
#define NIVEL_3 3 /* Int. Reloj */

/* Much more readable eXecution Levels */
#define XL_CLK NIVEL_3
#define XL_TRM NIVEL_2
#define XL_SW  NIVEL_1
#define XL_USR NIVEL_0

/* Process priority minimum and maximum levels */
#define MIN_PRIO 10
#define MAX_PRIO 50

/*
 *
 *  Definicion de constantes relacionadas con vectores de interrupcion
 *
 */

/* Numero de vectores de interrupcion disponibles */
#define NVECTORES 6

/* Numeros de vector */
#define EXC_ARITM 0     /* excepcion aritmetica */
#define EXC_MEM 1       /* excepcion en acceso a memoria */
#define INT_RELOJ 2     /* interrupcion de reloj */
#define INT_TERMINAL 3  /* interrupcion de entrada de terminal */
#define LLAM_SIS 4      /* vector usado para llamadas */
#define INT_SW 5    /* vector usado para interrupciones software */

/* frecuencia de reloj requerida (ticks/segundo) */
#define TICK 100

/* dirección de puerto de E/S del terminal */
#define DIR_TERMINAL 1

#endif /* _CONST_H */

