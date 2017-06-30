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

int tam_mem, rc = 0, logger;
char *mem;
static int semaphore;

int escreve(int pos, char* buf, int tam){
        int i;
        char* sm;
        if (pos >= tam_mem || pos < 0 || tam > tam_mem || tam + pos > tam_mem) {
                printf("ERRO DE ESCRITA\n");
                return 0;
        }
        for (i = pos; i < tam + pos; i++) {
                mem[i] = buf[i - pos];
        }
        //ATUALIZA A MEMORIA COMPARTILHADA
        sm = shmat(logger, 0, 0);
        for (i = 0; i < tam_mem; i++) {
                sm[i] = mem[i];
        }
        //show(sm,tam_mem + 1);
        return 1;
}

char* le(int pos, int tam){
        int i;
        char* buf;
        buf = (char *) calloc(tam,sizeof(char));
        if (pos >= tam_mem || pos < 0 || tam > tam_mem || tam + pos > tam_mem) {
                return NULL;
        }
        for (i = pos; i < tam + pos; i++) {
                buf[i - pos] = mem[i];
        }
        return buf;
}

void* Client(void* arg){
        Querry input;
        char *buf;
        int com, pos, tam, r;
        int sockEntrada = *(int *) arg;

        read(sockEntrada,&com,sizeof(int));
        read(sockEntrada,&pos,sizeof(int));
        read(sockEntrada,&tam,sizeof(int));

        input.com = ntohl(com);
        input.pos = ntohl(pos);
        input.tam = ntohl(tam);
        input.buf = (char*) malloc(input.tam*sizeof(char*));
        if(input.com == 0)
                read(sockEntrada,input.buf,input.tam);

        if (input.com == 0) { //ESCRITA NA MEMORIA
                printf("ESCRITA NA MEMORIA:\n");
                if (!db_p(semaphore)) exit(EXIT_FAILURE);
                /*CRITICA*/
                r = htonl(escreve(input.pos,input.buf,input.tam));
                show(mem,tam_mem);
                write(sockEntrada,&r,sizeof(int));
                /*CRITICA*/
                if (!db_v(semaphore)) exit(EXIT_FAILURE);
        }
        if (input.com == 1) { //LEITURA DA MEMORIA
                printf("LEITURA DA MEMORIA\n");
                if (!mutex_p(semaphore)) exit(EXIT_FAILURE);
                rc++;
                if(rc == 1) {
                        if (!db_p(semaphore)) exit(EXIT_FAILURE);
                }
                if (!mutex_v(semaphore)) exit(EXIT_FAILURE);
                /*CRITICA*/
                buf = (char*) malloc(input.tam*sizeof(char*));
                buf = le(input.pos,input.tam);
                write(sockEntrada,buf,input.tam);
                /*CRITICA*/
                if (!mutex_p(semaphore)) exit(EXIT_FAILURE);
                rc--;
                if(rc == 0) {
                        if (!db_v(semaphore)) exit(EXIT_FAILURE);
                }
                if (!mutex_v(semaphore)) exit(EXIT_FAILURE);
        }
        close(sockEntrada);
        pthread_exit((void*) 0);
}

int main(int argc, char *argv[]) {
        int server_sockfd, client_sockfd, server_len, client_len, t = 0, len = 0;
        struct sockaddr_in server_address, client_address;
        pthread_t *thread;

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
        thread = (pthread_t *) malloc(sizeof(pthread_t));

        //INICIALIZA OS SEMAFOROS
        semaphore = semget((key_t)id, 2, 0666 | IPC_CREAT);
        if (!set_semvalue(semaphore)) exit(EXIT_FAILURE);

        //INICIALIZA A MEMORIA COMPARTILHADA
        logger = shmget(ftok("server1.c", id), (size_t) tam_mem, IPC_CREAT | 0666);

        //BIND E LISTEN NA REDE
        bind(server_sockfd, (struct sockaddr*)&server_address, server_len);
        listen(server_sockfd, 20);
        printf("Server Esperando...\n");

        while(1) {
                client_len = sizeof(client_address);
                client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
                printf("CONECTADO COM %s\n",inet_ntoa(client_address.sin_addr));
                pthread_create(&(thread[t]), NULL, Client, &client_sockfd);
                t++;
                thread = (pthread_t *) realloc(thread,(t+1)*sizeof(pthread_t));
        }
        return 0;
}
