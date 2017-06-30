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

typedef struct srvr {
        char ip[16];
        int port;
} Server;

int tam_mem, n, count = 0, countfile = 0;
char *mem;

Server* readfile(){
        int i;
        char line[100];
        FILE *conf;
        Server *part;

        n = 1;
        conf = fopen("conf1.txt","r");
        fscanf(conf,"%d",&tam_mem);
        part = (Server*)malloc(n*sizeof(Server));
        fscanf(conf,"%s", line);
        while(1) {
                i = 0;
                while (1) {
                        if (line[i]==':') {
                                line[i] = ' ';
                                break;
                        }
                        i++;
                }
                sscanf(line,"%s %d",part[n-1].ip, &(part[n-1].port));
                if(fscanf(conf,"%s", line)==1) {
                        n++;
                        part = (Server*)realloc(part,n*sizeof(Server));
                } else {
                        break;
                }
        }
        fclose(conf);
        tam_mem = tam_mem / n;
        return part;
}

void receive(Server s){
        int com, pos, tam, sockfd, len, result, wr, i;
        struct sockaddr_in address;
        char arquivo[100];
        FILE *data;

        //DECLARACAO TCP
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        address.sin_family = AF_UNIX;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(s.ip);
        address.sin_port = htons(s.port);

        sprintf(arquivo,"logs/log[%d].txt",countfile);
        data = fopen(arquivo,"a");
        len = sizeof(address);
        result = connect(sockfd, (struct sockaddr*)&address, len);
        if(result == -1) {
                perror("ERRO DE CONEXAO\n");
                for (i = 0; count < tam_mem; count++) {
                        fprintf(data,"[%d] = inacessivel\n",count);
                        count++;
                }
                return;
        }
        mem = (char *) calloc(tam_mem,sizeof(char));
        read(sockfd, mem, tam_mem);

        for (i = 0; i < tam_mem; i++) {
                if(mem[i]==0) {
                        fprintf(data,"[%d] = \n",count);
                } else {
                        fprintf(data,"[%d] = %c\n",count,mem[i]);
                }
                count++;
        }
        fclose(data);
        close(sockfd);
}

int main(int argc, char *argv[]) {
        int server_sockfd, client_sockfd, server_len, client_len, t = 0, len = 0, i;
        struct sockaddr_in server_address, client_address;
        Server *part;
        int tempo = atoi(argv[1]);

        part = readfile();
        while (1) {
                count = 0;
                printf("log[%d].txt",countfile);
                for (i = 0; i < n; i++) {
                        receive(part[i]);
                }
                countfile++;
                sleep(tempo);
        }
        return 0;
}
