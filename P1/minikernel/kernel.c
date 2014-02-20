/*
 *  kernel/kernel.c
 *
 *  Minikernel. Versión 1.0
 *
 *  Fernando Pérez Costoya
 *
 */

/*
 *
 * Fichero que contiene la funcionalidad del sistema operativo
 *
 */

#include "kernel.h" /* Contiene defs. usadas por este modulo */

/**
 * User defined system and internal functions declarations
 */
static void block_process();
static void unblock_process(BCP* bcp);
static void swap_process(BCP *p_proc_anterior);

int sys_get_current_pid();
int sys_sleep();

/*
 *
 * Funciones relacionadas con la tabla de procesos:
 *  iniciar_tabla_proc buscar_BCP_libre
 *
 */

/*
 * Función que inicia la tabla de procesos
 */
static void iniciar_tabla_proc(){
    int i;

    for (i=0; i<MAX_PROC; i++)
        tabla_procs[i].estado=NO_USADA;
}

/*
 * Función que busca una entrada libre en la tabla de procesos
 */
static int buscar_BCP_libre(){
    int i;

    for (i=0; i<MAX_PROC; i++)
        if (tabla_procs[i].estado==NO_USADA)
            return i;
    return -1;
}

/*
 *
 * Funciones que facilitan el manejo de las listas de BCPs
 *  insertar_ultimo eliminar_primero eliminar_elem
 *
 * NOTA: PRIMERO SE DEBE LLAMAR A eliminar Y LUEGO A insertar
 */

/*
 * Inserta un BCP al final de la lista.
 */
static void insertar_ultimo(lista_BCPs *lista, BCP * proc){
    if (lista->primero==NULL)
        lista->primero= proc;
    else
        lista->ultimo->siguiente=proc;
    lista->ultimo= proc;
    proc->siguiente=NULL;
}

/*
 * Elimina el primer BCP de la lista.
 */
static void eliminar_primero(lista_BCPs *lista){

    if (lista->ultimo==lista->primero)
        lista->ultimo=NULL;
    lista->primero=lista->primero->siguiente;
}

/*
 * Elimina un determinado BCP de la lista.
 */
static void eliminar_elem(lista_BCPs *lista, BCP * proc){
    BCP *paux=lista->primero;

    if (paux==proc)
        eliminar_primero(lista);
    else {
        for ( ; ((paux) && (paux->siguiente!=proc));
            paux=paux->siguiente);
        if (paux) {
            if (lista->ultimo==paux->siguiente)
                lista->ultimo=paux;
            paux->siguiente=paux->siguiente->siguiente;
        }
    }
}

/*
 *
 * Funciones relacionadas con la planificacion
 *  espera_int planificador
 */

/*
 * Espera a que se produzca una interrupcion
 */
static void espera_int(){
    int nivel;

    /* printk("[KRN][-1] NO HAY LISTOS. ESPERA INT\n"); */

    /* Baja al mínimo el nivel de interrupción mientras espera */
    nivel=fijar_nivel_int(NIVEL_1);
    halt();
    fijar_nivel_int(nivel);
}

/*
 * Función de planificacion que implementa un algoritmo FIFO.
 */
static BCP * planificador(){
    while (lista_listos.primero==NULL)
        espera_int();       /* No hay nada que hacer */

    if ( p_proc_actual )
        printk("[KRN][%2d] SCHEDULLING NEXT PROCESS\n", p_proc_actual->id);
    else
        printk("[KRN][-1] SCHEDULLING NEXT PROCESS\n");
    
    return lista_listos.primero;
}

/*
 *
 * Funcion auxiliar que termina proceso actual liberando sus recursos.
 * Usada por llamada terminar_proceso y por rutinas que tratan excepciones
 *
 */
static void liberar_proceso(){
    BCP * p_proc_anterior;

    printk("[KRN][%2d] FREEING PROCESS\n", p_proc_actual->id);

    liberar_imagen(p_proc_actual->info_mem); /* liberar mapa */
    /*
    If "liberar_imagen" is called without any image on memory, the Kernel
    will be halted and execution will stop.
    */

    p_proc_actual->estado=TERMINADO;
    eliminar_primero(&lista_listos); /* proc. fuera de listos */

    /* Realizar cambio de contexto */
    p_proc_anterior=p_proc_actual;

    swap_process(p_proc_anterior);
        
        return; /* no debería llegar aqui */
}

static void block_process()
{
    BCP* p_proc_anterior;
    BCP* p_proc_siguiente;

    printk("[KRN][%2d] BLOCKING PROCESS: [%d]\n", p_proc_actual->id, p_proc_actual->id);

    fijar_nivel_int(XL_SW); /* NIVEL_1 */

    /* Remove the top process from the ready list, and mark it as BLOCKED. */
    p_proc_actual->estado = BLOQUEADO;
    p_proc_anterior = p_proc_actual;
    eliminar_primero(&lista_listos);

    /* Insert the process to sleep in the list */
    insertar_ultimo(&l_slept_procs, p_proc_anterior);

    p_proc_siguiente = planificador();

    printk("[KRN][%2d] CHANGING CONTEXT: [%d] => [%d]\n", p_proc_actual->id, p_proc_anterior->id, p_proc_siguiente->id);

    p_proc_actual = p_proc_siguiente;

    liberar_pila(p_proc_anterior->pila);
    
    /* "cambio_contexto" is in charge of rising the interruption level to XL_CLK. */
    /*
     * Do save old context to be swapped, so that it is
     * possible to restore it after being awaken.
     */
    cambio_contexto(&(p_proc_anterior->contexto_regs), &(p_proc_actual->contexto_regs));

        return;
}

static void unblock_process(BCP* bcp)
{
    BCP* p_proc_anterior;

    printk("[KRN][%2d] AWAKENING PROCESS: [%d]\n", p_proc_actual->id, bcp->id);

    fijar_nivel_int(XL_SW); /* NIVEL_1 */

    /* Remove the awaken bcp from the slept process list */
    printk("[KRN][%2d] REMOVING PROCESS FROM THE SLEPT LIST\n", p_proc_actual->id);    
    eliminar_elem(&l_slept_procs, bcp);
    
    /* Add the awaken bcp to the first one on the ready to be processed list */
    printk("[KRN][%2d] ADDING THE AWAKEN PROCESS TO THE READY LIST\n", p_proc_actual->id);    
    p_proc_anterior = lista_listos.primero;
    lista_listos.primero = bcp;
    bcp->siguiente = p_proc_anterior;
    bcp->estado = LISTO;

    swap_process(p_proc_anterior);

        return;
}

/*
 * Given an old process control block, swap the context
 * to a new one, and free the heap of the old one.
 */
static void swap_process(BCP *p_proc_anterior)
{
    BCP* p_proc_siguiente;
    
    p_proc_siguiente = planificador();

    if ( p_proc_anterior != NULL )
        printk("[KRN][%2d] CHANGING CONTEXT: [%d] => [%d]\n", p_proc_actual->id, p_proc_anterior->id, p_proc_siguiente->id);
    else
        printk("[KRN][%2d] CHANGING CONTEXT: [-1] => [%d]\n", p_proc_actual->id, p_proc_siguiente->id);

    p_proc_actual = p_proc_siguiente;

    if ( p_proc_anterior != NULL )
        liberar_pila(p_proc_anterior->pila);
    
    /* "cambio_contexto" is in charge of rising the interruption level to XL_CLK. */
    cambio_contexto(NULL, &(p_proc_actual->contexto_regs));
}

static void update_slept_process()
{
    /* printk("[KRN][%2d] Updating slept process\n", p_proc_actual->id); */

    BCP* bcp = l_slept_procs.primero;
    for ( ; bcp ; bcp = bcp->siguiente )
    {
        /*
         * Little issue: Unblocking a process leads to a change of context.
         * What happens to other sleeping process that during the moment
         * of unblocking a process, can also be unblocked?
         * 
         * For now, they have to wait for the next clock interruption...
         */
        bcp->tts--;
        /* printk("[KRN][%2d] BCP[%2d]->tts = [%d]\n", p_proc_actual->id, bcp->id, bcp->tts); */
        if ( !bcp->tts )
            unblock_process(bcp);
    }
}

/*
 *
 * Funciones relacionadas con el tratamiento de interrupciones
 *  excepciones: exc_arit exc_mem
 *  interrupciones de reloj: int_reloj
 *  interrupciones del terminal: int_terminal
 *  llamadas al sistemas: llam_sis
 *  interrupciones SW: int_sw
 *
 */

/*
 * Tratamiento de excepciones aritmeticas
 */
static void exc_arit(){

    if (!viene_de_modo_usuario())
        panico("[KRN][-1] >> KERNEL_EXCEPTION [Invalid arithmetic operation] <<");

    printk("[KRN][%2d] >> EXCEPTION [Invalid arithmetic operation] <<\n", p_proc_actual->id);
    liberar_proceso();

        return; /* no debería llegar aqui */
}

/*
 * Tratamiento de excepciones en el acceso a memoria
 */
static void exc_mem(){

    if (!viene_de_modo_usuario())
    {
        if ( p_proc_actual )
            panico("[KRN][-1] >> KERNEL_EXCEPTION [Invalid memory access] <<");
    }

    printk("[KRN][%2d] >> EXCEPTION [Invalid memory access] <<\n", p_proc_actual->id);
    liberar_proceso();

        return; /* no debería llegar aqui */
}

/*
 * Tratamiento de interrupciones de terminal
 */
static void int_terminal(){

    printk("[KRN][%2d] TRATANDO INT. DE TERMINAL %c\n", p_proc_actual->id, leer_puerto(DIR_TERMINAL));

        return;
}

/*
 * Tratamiento de interrupciones de reloj
 */
static void int_reloj(){

    /* printk("[KRN][%2d] >> INTERRUPTION [Clock] <<\n", p_proc_actual->id); */

    update_slept_process();

        return;
}

/*
 * Tratamiento de llamadas al sistema
 */
static void tratar_llamsis(){
    int nserv, res;

    nserv=leer_registro(0);
    if (nserv<NSERVICIOS)
        res=(tabla_servicios[nserv].fservicio)();
    else
        res=-1;     /* servicio no existente */
    escribir_registro(0,res);
    return;
}

/*
 * Tratamiento de interrupciuones software
 */
static void int_sw(){

    printk("[KRN] TRATANDO INT. SW\n");

    return;
}

/*
 *
 * Funcion auxiliar que crea un proceso reservando sus recursos.
 * Usada por llamada crear_proceso.
 *
 */
static int crear_tarea(char *prog){
    void * imagen, *pc_inicial;
    int error=0;
    int proc;
    BCP *p_proc;

    proc=buscar_BCP_libre();
    if (proc==-1)
        return -1;  /* no hay entrada libre */

    /* A rellenar el BCP ... */
    p_proc=&(tabla_procs[proc]);

    /* crea la imagen de memoria leyendo ejecutable */
    imagen=crear_imagen(prog, &pc_inicial);
    if (imagen)
    {
        p_proc->info_mem=imagen;
        p_proc->pila=crear_pila(TAM_PILA);
        fijar_contexto_ini(p_proc->info_mem, p_proc->pila, TAM_PILA,
            pc_inicial,
            &(p_proc->contexto_regs));
        p_proc->id=proc;
        p_proc->estado=LISTO;

        /* lo inserta al final de cola de listos */
        insertar_ultimo(&lista_listos, p_proc);
        error= 0;
        
        if ( p_proc_actual )
            printk("[KRN][%2d] LOADED IMAGE [%s]\n", p_proc_actual->id, prog);
        else
            printk("[KRN][-1] LOADED IMAGE [%s]\n", prog);
    }
    else
    {
        error= -1; /* fallo al crear imagen */
        printk("[KRN][-1] >> ERROR [Could not load image [%s]] <<\n", prog);
    }

    return error;
}

/*
 *
 * Rutinas que llevan a cabo las llamadas al sistema
 *  sis_crear_proceso sis_escribir
 *
 */

/*
 * Tratamiento de llamada al sistema crear_proceso. Llama a la
 * funcion auxiliar crear_tarea sis_terminar_proceso
 */
int sis_crear_proceso(){
    char *prog;
    int res;

    prog = (char *)leer_registro(1);
    printk("[KRN][%2d] CREATING PROCESS [%s]\n", p_proc_actual->id, prog);
    res=crear_tarea(prog);
    return res;
}

/*
 * Tratamiento de llamada al sistema escribir. Llama simplemente a la
 * funcion de apoyo escribir_ker
 */
int sis_escribir()
{
    char *texto;
    unsigned int longi;

    texto=(char *)leer_registro(1);
    longi=(unsigned int)leer_registro(2);

    escribir_ker(texto, longi);
    return 0;
}

/*
 * Tratamiento de llamada al sistema terminar_proceso. Llama a la
 * funcion auxiliar liberar_proceso
 */
int sis_terminar_proceso(){

    printk("[KRN][%2d] ENDING PROCESS\n", p_proc_actual->id);

    liberar_proceso();

        return 0; /* no debería llegar aqui */
}

/**
 * Returns the current process identifier.
 */
int sys_get_current_pid()
{
    return p_proc_actual->id;
}

/**
 * Puts the current process to sleep and proceeds to
 * the next one until the current process awakens.
 */
int sys_sleep()
{
    /* Update process ticks to sleep  */
    p_proc_actual->tts = (unsigned int) leer_registro(1) * TICK;
    /* p_proc_actual->tts = (unsigned int) leer_registro(1); */
    printk("[KRN][%2d] PUTTING TO SLEEP CURRENT PROCESS FOR [%d] TICKS\n", p_proc_actual->id, p_proc_actual->tts);

    /* Block the current process */
    block_process();
    
        return 0; /* WOOOPS */
}

/*
 *
 * Rutina de inicialización invocada en arranque
 *
 */
int main(){
    /* se llega con las interrupciones prohibidas */
    iniciar_tabla_proc();

    instal_man_int(EXC_ARITM, exc_arit); 
    instal_man_int(EXC_MEM, exc_mem); 
    instal_man_int(INT_RELOJ, int_reloj); 
    instal_man_int(INT_TERMINAL, int_terminal); 
    instal_man_int(LLAM_SIS, tratar_llamsis); 
    instal_man_int(INT_SW, int_sw); 

    iniciar_cont_int();         /* inicia cont. interr. */
    iniciar_cont_reloj(TICK);   /* fija frecuencia del reloj */
    iniciar_cont_teclado();     /* inici cont. teclado */

    /* crea proceso inicial */
    printk("[KRN][-1] INITIALIZING KERNEL\n");
    if (crear_tarea((void *) "init") < 0)
        panico("[KRN][-1] >> KERNEL_EXCEPTION [[init] image not found] <<");
    
    /* activa proceso inicial */
    p_proc_actual=planificador();
    cambio_contexto(NULL, &(p_proc_actual->contexto_regs));
    panico("[KRN][-1] >> KERNEL_EXCEPTION [S.O. reactivado inesperadamente] <<");
    return 0;
}
