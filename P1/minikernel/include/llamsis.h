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
#define NSERVICIOS 4

#define CREAR_PROCESO 0
#define TERMINAR_PROCESO 1
#define ESCRIBIR 2

#define GET_PID 3

#endif /* _LLAMSIS_H */

