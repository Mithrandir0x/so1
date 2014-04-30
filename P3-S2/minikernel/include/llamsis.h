/*
 *  minikernel/kernel/include/llamsis.h
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 * Fichero de cabecera que contiene el numero asociado a cada llamada
 */

#ifndef _LLAMSIS_H
#define _LLAMSIS_H

/* Numero de servicios disponibles */
#define NSERVICIOS         8

#define CREAR_PROCESO      0
#define TERMINAR_PROCESO   1
#define ESCRIBIR           2

#define SYS_ID_GET_PID     3
#define SYS_ID_SLEEP       4
#define SYS_GET_PRIORITY   5
#define SYS_GET_PARENT_PID 6
#define SYS_WAIT           7

#endif /* _LLAMSIS_H */

