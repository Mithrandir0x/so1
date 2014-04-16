/*
 *  usuario/include/servicios.h
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 *
 * Fichero de cabecera que contiene los prototipos de funciones de
 * biblioteca que proporcionan la interfaz de llamadas al sistema.
 *
 *      SE DEBE MODIFICAR AL INCLUIR NUEVAS LLAMADAS
 *
 */

#ifndef SERVICIOS_H
#define SERVICIOS_H

/* Evita el uso del printf de la bilioteca estándar */
#define printf escribirf

/* Funcion de biblioteca */
int escribirf(const char *formato, ...);

/* Llamadas al sistema proporcionadas */
int crear_proceso(char *prog);
int terminar_proceso();
int escribir(char *texto, unsigned int longi);

int srv_get_current_process_id();
int srv_sleep(unsigned int seconds);
int srv_set_priority(unsigned int priority);
int srv_get_parent_pid();

#endif /* SERVICIOS_H */

