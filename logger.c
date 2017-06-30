#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/sem.h>
#include "semun.h"
#include "parser.h"
#include <sys/ipc.h>
#include <sys/shm.h>

int tam_mem, logger;
char *mem;
static int semaphore;

int main(int argc, char const *argv[]) {

        int server_sockfd, client_sockfd, server_len, client_len, t = 0, len = 0;
        struct sockaddr_in server_address, client_address;

        //PARAMETROS
        int port = atoi(argv[3]);
        int id = atoi(argv[2]);
        tam_mem = atoi(argv[1]);

        //DECLARACAO TCP
        server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr("0.0.0.0");
        server_address.sin_port = htons(port);
        server_len = sizeof(server_address);

        //INICIALIZA A MEMORIA

        mem = (char *) calloc(tam_mem,sizeof(char));

        //INICIALIZA OS SEMAFOROS
        semaphore = semget((key_t)id, 2, 0666 | IPC_CREAT);
        //INICIALIZA A MEMORIA COMPARTILHADA
        logger = shmget(ftok("server1.c", id), (size_t) tam_mem, IPC_CREAT | 0666);

        //BIND E LISTEN NA REDE
        bind(server_sockfd, (struct sockaddr*)&server_address, server_len);
        listen(server_sockfd, 20);
        printf("Logger executando...\n");

        while(1) {
                client_len = sizeof(client_address);
                client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
                printf("CONECTADO COM MESTRE %s\n",inet_ntoa(client_address.sin_addr));
                if (!db_p(semaphore)) exit(EXIT_FAILURE);
                /*CRITICA*/
                mem = (char*) shmat(logger, 0, 0);
                show(mem,tam_mem);
                /*CRITICA*/
                if (!db_v(semaphore)) exit(EXIT_FAILURE);
                write(client_sockfd, mem, tam_mem);
        }
        return 0;
}
