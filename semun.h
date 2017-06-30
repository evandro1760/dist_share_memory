#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
    /* union semun is defined by including <sys/sem.h> */
#else
    /* according to X/OPEN we have to define it ourselves */
    union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
    };
#endif

/*
//int semget(key_t key, int nsems, int semflg) -> para criar um semaforo
static int set_semvalue(int semaphore);  // para inicializar um semaforo
static void del_semvalue(int semaphore); // para deletar um semaforo
static int semaphore_p(int semaphore);   // para esperar pelo acesso
static int semaphore_v(int semaphore);   // para liberar o acesso
*/

static int set_semvalue(int semaphore)
{
        union semun sem_union;
        sem_union.val = 1;

        if (semctl(semaphore, 0, SETVAL, sem_union) == -1) {
                fprintf(stderr, "semaphore_1 falhou\n");
                return(0);
        }
        if (semctl(semaphore, 1, SETVAL, sem_union) == -1) {
                fprintf(stderr, "semaphore_2 falhou\n");
                return(0);
        }
        return(1);
}
static void del_semvalue(int semaphore)
{
        union semun sem_union;

        if (semctl(semaphore, 0, IPC_RMID, sem_union) == -1)
                fprintf(stderr, "Falha ao remover o semaforo\n");
}
static int db_p(int semaphore)
{
        struct sembuf sem_b;

        sem_b.sem_num = 0;
        sem_b.sem_op = -1; /* P() */
        sem_b.sem_flg = SEM_UNDO;
        if (semop(semaphore, &sem_b, 1) == -1) {
                fprintf(stderr, "db_p falhou\n");
                return(0);
        }
        return(1);
}
static int db_v(int semaphore)
{
        struct sembuf sem_b;

        sem_b.sem_num = 0;
        sem_b.sem_op = 1; /* V() */
        sem_b.sem_flg = SEM_UNDO;
        if (semop(semaphore, &sem_b, 1) == -1) {
                fprintf(stderr, "db_v falhou\n");
                return(0);
        }
        return(1);
}
static int mutex_p(int semaphore)
{
        struct sembuf sem_b;

        sem_b.sem_num = 1;
        sem_b.sem_op = -1; /* P() */
        sem_b.sem_flg = SEM_UNDO;
        if (semop(semaphore, &sem_b, 1) == -1) {
                fprintf(stderr, "mutex_p falhou\n");
                return(0);
        }
        return(1);
}
static int mutex_v(int semaphore)
{
        struct sembuf sem_b;

        sem_b.sem_num = 1;
        sem_b.sem_op = 1; /* V() */
        sem_b.sem_flg = SEM_UNDO;
        if (semop(semaphore, &sem_b, 1) == -1) {
                fprintf(stderr, "mutex_v falhou\n");
                return(0);
        }
        return(1);
}
