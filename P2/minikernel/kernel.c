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
 * BCP list manipulation function declarations
 */
static void iniciar_tabla_proc();
static int buscar_BCP_libre();
static void insertar_ultimo(lista_BCPs *lista, BCP * proc);
static void eliminar_primero(lista_BCPs *lista);
static void eliminar_elem(lista_BCPs *lista, BCP * proc);
static void print_bcp_list(lista_BCPs *list);
static char* get_state_string(int state);
static void print_bcp(BCP *bcp);

/**
 * Scheduler related function declarations
 */
static BCP* maxima_prioridad(lista_BCPs *list);
static void espera_int();
static BCP * planificador();

/**
 * User defined system and internal function declarations
 */
static int crear_tarea(char *prog);
static void liberar_proceso();
static void block_process();
static void unblock_process(BCP* bcp);

/**
 * Interruption treatment and auxiliary function declarations
 */
static void update_slept_process();
static void exc_arit();
static void exc_mem();
static void int_terminal();
static void int_reloj();
static void tratar_llamsis();
static void int_sw();

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

/**
 * Print a BCP list.
 */
static void print_bcp_list(lista_BCPs *list)
{
    int i = 0;
    BCP *p = list->primero;

    if ( p_proc_actual )
        printk("[KRN][%2d][%16.16s] {\n", p_proc_actual->id, "print_bcp_list");
    else
        printk("[KRN][-1][%16.16s] {\n", "print_bcp_list");

    for ( ; p ; p = p->siguiente )
    {
        print_bcp(p);
        i++;
    }

    if ( p_proc_actual )
        printk("[KRN][%2d][%16.16s] } Length: [%2d]\n", p_proc_actual->id, "print_bcp_list", i);
    else
        printk("[KRN][-1][%16.16s] } Length: [%2d]\n", "print_bcp_list", i);
}

/**
 * This function returns a string based on a state numerical identifier.
 */
static char* get_state_string(int state)
{
    switch ( state )
    {
        case 0: return "TERMINADO";
        case 1: return "LISTO";
        case 2: return "EJECUCION";
        case 3: return "BLOQUEADO";
    }

    return "UNKNOWN";
}

/**
 * Print a BCP.
 */
static void print_bcp(BCP *bcp)
{
    if ( p_proc_actual )
        printk("[KRN][%2d]", p_proc_actual->id);
    else
        printk("[KRN][-1]");

    printk("[%16.16s]   [%2d] {S: [%9s], P: [%2d]}\n",
        "print_bcp",
        bcp->id,
        get_state_string(bcp->estado),
        bcp->priority
    );
}

static BCP* maxima_prioridad(lista_BCPs *list)
{
    unsigned int max = MIN_PRIO;
    BCP *p = list->primero;
    BCP *p_max = p;
    
    for ( ; p ; p = p->siguiente )
    {
        if ( p->priority == MAX_PRIO ) 
            return p;
        
        if ( p->priority > max )
        {
            p_max = p;
            max = p->priority;
        }
    }

    return p_max;
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

    // if ( p_proc_actual )
    //     printk("[KRN][%2d][%16.16s] WAITING FOR INTERRUPTION\n", p_proc_actual->id, "espera_int");
    // else
    //     printk("[KRN][-1][%16.16s] WAITING FOR INTERRUPTION\n", "espera_int");

    /* Baja al mínimo el nivel de interrupción mientras espera */
    nivel=fijar_nivel_int(NIVEL_1);
    halt();
    fijar_nivel_int(nivel);
}

/*
 * Función de planificacion que implementa un algoritmo FIFO.
 */
static BCP * planificador(){
    if ( lista_listos.primero==NULL )
    {
        if ( p_proc_actual )
            printk("[KRN][%2d][%16.16s] WAITING FOR A PROCESS TO AWAKE\n", p_proc_actual->id, "planificador");
        else
            printk("[KRN][-1][%16.16s] WAITING FOR A PROCESS TO AWAKE\n", "planificador");
    }
    
    while (lista_listos.primero==NULL)
        espera_int();       /* No hay nada que hacer */

    if ( p_proc_actual )
        printk("[KRN][%2d][%16.16s] SCHEDULLING NEXT PROCESS\n", p_proc_actual->id, "planificador");
    else
        printk("[KRN][-1][%16.16s] SCHEDULLING NEXT PROCESS\n", "planificador");
    
    return maxima_prioridad(&lista_listos);
}

/*
 *
 * Funcion auxiliar que termina proceso actual liberando sus recursos.
 * Usada por llamada terminar_proceso y por rutinas que tratan excepciones
 *
 */
static void liberar_proceso(){
    BCP * p_proc_anterior;
    BCP * p_proc_siguiente;

    printk("[KRN][%2d][%16.16s] FREEING PROCESS\n", p_proc_actual->id, "liberar_proceso");

    /*
    If "liberar_imagen" is called without any image on memory, the Kernel
    will be halted and execution will stop.
    */
    liberar_imagen(p_proc_actual->info_mem); /* liberar mapa */

    p_proc_actual->estado=TERMINADO;
    eliminar_elem(&lista_listos, p_proc_actual);

    printk("[KRN][%2d][%16.16s] READY BCP LIST: \n", p_proc_actual->id, "liberar_proceso");
    print_bcp_list(&lista_listos);

    /* Realizar cambio de contexto */
    p_proc_anterior=p_proc_actual;
    p_proc_siguiente = planificador();

    printk("[KRN][%2d][%16.16s] CHANGING CONTEXT: [%d] => [%d]\n", p_proc_actual->id, "liberar_proceso", p_proc_anterior->id, p_proc_siguiente->id);

    p_proc_actual = p_proc_siguiente;

    liberar_pila(p_proc_anterior->pila);

    //printk("[KRN][%2d][%16.16s] MADE FREE PROCESS [%2d] STACK.\n", p_proc_actual->id, "liberar_proceso", p_proc_anterior->id);
    
    cambio_contexto(NULL, &(p_proc_actual->contexto_regs));
        
        return; /* no debería llegar aqui */
}

static void block_process()
{
    int old_int_level;
    BCP* p_proc_anterior;
    BCP* p_proc_siguiente;

    old_int_level = fijar_nivel_int(XL_CLK); /* NIVEL_3 */

    printk("[KRN][%2d][%16.16s] BLOCKING PROCESS: [%d]\n", p_proc_actual->id, "block_process", p_proc_actual->id);

    /* Remove the top process from the ready list, and mark it as BLOCKED. */
    p_proc_actual->estado = BLOQUEADO;
    p_proc_anterior = p_proc_actual;
    eliminar_primero(&lista_listos);

    /* Insert the process to sleep in the list */
    insertar_ultimo(&l_slept_procs, p_proc_anterior);

    printk("[KRN][%2d][%16.16s] SLEPT BCP LIST: \n", p_proc_actual->id, "block_process");
    print_bcp_list(&l_slept_procs);

    printk("[KRN][%2d][%16.16s] READY BCP LIST: \n", p_proc_actual->id, "block_process");
    print_bcp_list(&lista_listos);

    fijar_nivel_int(old_int_level);

    p_proc_siguiente = planificador();

    printk("[KRN][%2d][%16.16s] CHANGING CONTEXT: [%d] => [%d]\n", p_proc_actual->id, "block_process", p_proc_anterior->id, p_proc_siguiente->id);

    p_proc_actual = p_proc_siguiente;

    liberar_pila(p_proc_anterior->pila);

    f_pending_schedulling = false;
    
    /* "cambio_contexto" is in charge of rising the interruption level to XL_CLK. */
    /*
     * Do save old context to be swapped, so that it is
     * possible to restore it after being awaken.
     */
    cambio_contexto(&(p_proc_anterior->contexto_regs), &(p_proc_actual->contexto_regs));
}

static void unblock_process(BCP* bcp)
{
    int old_int_level;

    old_int_level = fijar_nivel_int(XL_CLK); /* NIVEL_3 */

    printk("[KRN][%2d][%16.16s] AWAKENING PROCESS: [%d]\n", p_proc_actual->id, "unblock_process", bcp->id);

    /* Remove the awaken bcp from the slept process list */
    printk("[KRN][%2d][%16.16s] REMOVING PROCESS FROM THE SLEPT LIST\n", p_proc_actual->id, "unblock_process");    
    eliminar_elem(&l_slept_procs, bcp);
    
    /* Add the awaken bcp to the first one on the ready to be processed list */
    printk("[KRN][%2d][%16.16s] ADDING THE AWAKEN PROCESS TO THE READY LIST\n", p_proc_actual->id, "unblock_process");
    bcp->estado = LISTO;
    insertar_ultimo(&lista_listos, bcp);

    printk("[KRN][%2d][%16.16s] SLEPT BCP LIST: \n", p_proc_actual->id, "unblock_process");
    print_bcp_list(&l_slept_procs);

    printk("[KRN][%2d][%16.16s] READY BCP LIST: \n", p_proc_actual->id, "unblock_process");
    print_bcp_list(&lista_listos);

    fijar_nivel_int(old_int_level);
}

static void update_slept_process()
{
    //printk("[KRN][%2d][%16.16s] Updating slept process\n", p_proc_actual->id, "update_slept_process");

    BCP* bcp = l_slept_procs.primero;
    BCP* next_bcp = NULL;
    
    while ( bcp )
    {
        bcp->tts--;
        next_bcp = bcp->siguiente;

#ifdef __KRN_DBG_UPDATE_SLEPT_PROCESS__
        printk("[KRN][%2d][%16.16s] BCP[%2d]->tts = [%d]\n", p_proc_actual->id, "update_slept_process", bcp->id, bcp->tts);
#endif
        if ( !bcp->tts )
        {
            unblock_process(bcp);

            if ( p_proc_actual->priority < bcp->priority )
            {
                printk("[KRN][%2d][%16.16s] PROCESS [%2d] WITH HIGHER PRIORITY MUST BE AWAKEN\n", p_proc_actual->id, "update_slept_process", bcp->id);

                f_pending_schedulling = true;
                activar_int_SW();
            }

            bcp = next_bcp;
        }
        else
        {
            bcp = bcp->siguiente;
        }
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
        panico("[KRN][-1][        exc_arit] >> KERNEL_EXCEPTION [Invalid arithmetic operation] <<");

    printk("[KRN][%2d][%16.16s] >> EXCEPTION [Invalid arithmetic operation] <<\n", p_proc_actual->id, "exc_arit");
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
            panico("[KRN][-1][         exc_mem] >> KERNEL_EXCEPTION [Invalid memory access] <<");
    }

    printk("[KRN][%2d][%16.16s] >> EXCEPTION [Invalid memory access] <<\n", p_proc_actual->id, "exc_mem");
    liberar_proceso();

        return; /* no debería llegar aqui */
}

/*
 * Tratamiento de interrupciones de terminal
 */
static void int_terminal(){

    printk("[KRN][%2d][%16.16s] TRATANDO INT. DE TERMINAL [%c]\n", p_proc_actual->id, "int_terminal", leer_puerto(DIR_TERMINAL));

        return;
}

/*
 * Tratamiento de interrupciones de reloj
 */
static void int_reloj(){

    //printk("[KRN][%2d] >> INTERRUPTION [Clock] <<\n", p_proc_actual->id);

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
static void int_sw()
{
    BCP *p_proc_anterior = NULL;

    printk("[KRN][%2d][%16.16s] TREATING SOFTWARE INTERRUPTION\n", p_proc_actual->id, "int_sw");

    if ( f_pending_schedulling )
    {
        p_proc_anterior = p_proc_actual;
        liberar_pila(p_proc_anterior->pila);
        p_proc_actual = planificador();
        
        printk("[KRN][%2d][%16.16s] PRIORITIZED PROCESS. CHANGING CONTEXT: [%2d] => [%2d]\n", p_proc_actual->id, "int_sw", p_proc_anterior->id, p_proc_actual->id);

        printk("[KRN][%2d][%16.16s] READY BCP LIST: \n", p_proc_actual->id, "int_sw");
        print_bcp_list(&lista_listos);

        cambio_contexto(&(p_proc_anterior->contexto_regs), &(p_proc_actual->contexto_regs));
    }

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

        if ( p_proc_actual )
            p_proc->priority = p_proc_actual->priority;
        else
            p_proc->priority = MIN_PRIO;

        /* lo inserta al final de cola de listos */
        insertar_ultimo(&lista_listos, p_proc);
        error= 0;
        
        if ( p_proc_actual )
            printk("[KRN][%2d][%16.16s] LOADED IMAGE [%s] WITH PID [%2d]\n", p_proc_actual->id, "crear_tarea", prog, proc);
        else
            printk("[KRN][-1][%16.16s] LOADED IMAGE [%s] WITH PID [%2d]\n", "crear_tarea", prog, proc);
    }
    else
    {
        error = -1; /* fallo al crear imagen */
        
        if ( p_proc_actual )
            printk("[KRN][%2d][%16.16s] >> ERROR [Could not load image [%s]] <<\n", p_proc_actual->id, "crear_tarea", prog, proc);
        else
            printk("[KRN][-1][%16.16s] >> ERROR [Could not load image [%s]] <<\n", "crear_tarea", prog, proc);
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
    printk("[KRN][%2d][%16.16s] CREATING PROCESS [%s]\n", p_proc_actual->id, "sis_crear_proceso", prog);
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

    printk("[KRN][%2d][%16.16s] ENDING PROCESS\n", p_proc_actual->id, "sis_terminar_proceso");

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
    printk("[KRN][%2d][%16.16s] PUTTING TO SLEEP CURRENT PROCESS FOR [%d] TICKS\n", p_proc_actual->id, "sys_sleep", p_proc_actual->tts);

    /* Block the current process */
    block_process();
    
        return 0; /* WOOOPS */
}

/**
 * Sets the priority of the current process to a new one.
 */
int sys_set_priority()
{
    int priority = MIN_PRIO;

    priority = (unsigned int) leer_registro(1);

    if ( MIN_PRIO > priority || MAX_PRIO < priority )
    {
        printk("[KRN][%2d][%16.16s] INVALID PRIORITY VALUE\n", p_proc_actual->id, "sys_set_priority");
        return -1;
    }

    // Should a process that lowers its own priority be shoop-da-wooped for another process with more priority?
    if ( priority < p_proc_actual->priority )
    {
        printk("[KRN][%2d][%16.16s] PROCESS PRIORITY HAS BEEN LOWERED. TRIGGERING ICC.\n", p_proc_actual->id, "sys_set_priority");
        
        f_pending_schedulling = true;
        activar_int_SW();
    }

    p_proc_actual->priority = priority;

    printk("[KRN][%2d][%16.16s] SETTING NEW PRIORITY: [%d]\n", p_proc_actual->id, "sys_set_priority", priority);

    return 0;
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
    printk("[KRN][-1][            main] INITIALIZING KERNEL\n");
    if (crear_tarea((void *) "init") < 0)
        panico("[KRN][-1][            main] >> KERNEL_EXCEPTION [[init] image not found] <<");
    
    /* activa proceso inicial */
    p_proc_actual=planificador();
    cambio_contexto(NULL, &(p_proc_actual->contexto_regs));
    panico("[KRN][-1][            main] >> KERNEL_EXCEPTION [S.O. reactivado inesperadamente] <<");
    return 0;
}
