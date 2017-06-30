#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "parser.h"

typedef struct srvr {
        char ip[16];
        int port;
} Server;

int tam_mem, n;
char *buf;

Server* readfile(){
        int i;
        char line[100];
        FILE *conf;
        Server *part;

        n = 1;
        conf = fopen("conf.txt","r");
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
        return part;
}

void send_querry(Server s, Querry input){
        int com, pos, tam, sockfd, len, result, wr;
        struct sockaddr_in address;
        //char *buf;

        //DECLARACAO TCP
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        address.sin_family = AF_UNIX;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(s.ip);
        address.sin_port = htons(s.port);

        len = sizeof(address);
        result = connect(sockfd, (struct sockaddr*)&address, len);
        if(result == -1) {
                perror("ERRO DE CONEXAO\n");
                return;
        }

        com = htonl(input.com);
        pos = htonl(input.pos);
        tam = htonl(input.tam);

        write(sockfd,&com,sizeof(int));
        write(sockfd,&pos,sizeof(int));
        write(sockfd,&tam,sizeof(int));
        if(input.com == 0)
                write(sockfd,input.buf,input.tam);

        if(input.com == 1) {
                buf = (char*) malloc(input.tam*sizeof(char*));
                read(sockfd,buf,input.tam);
                show(buf,input.tam);
        }
        if(input.com == 0) {
                read(sockfd,&wr,sizeof(int));
                wr = ntohl(wr);
                if (wr) {
                        printf("\nESCRITA REALIZADA COM SUCESSO\n");
                } else {
                        printf("\nERRO NA ESCRITA\n");
                }
        }
        close(sockfd);
}

char* divstr(char* str, int a, int b){
        int i, j=0;
        char* out;
        out = (char*)malloc((b-a+1)*sizeof(char));
        for (i = a; i <= b; i++) {
                out[j] = str[i];
                j++;
        }
        return out;
}

int main(){
        int sockfd, len, result,i;
        int x, pos, tam;
        struct sockaddr_in address;
        char *msg, *ped;
        Querry input, output;
        Server *part;

        part = readfile();

        while (1) {
                msg = (char*)malloc((tam_mem + 20)*sizeof(char));
                fgets(msg, tam_mem + 20, stdin);
                fflush(stdin);
                input = parser(msg, tam_mem + 20);

                //show_querry(input);
                output.com = input.com;
                while (1) {
                        x = (input.pos) / (tam_mem/n);
                        output.pos = input.pos - (x * (tam_mem/n));
                        tam = (tam_mem/n) - output.pos;
                        if (tam < input.tam) {
                                output.tam = tam;
                                if(output.com == 0) {
                                        output.buf = divstr(input.buf,0,tam-1);
                                        input.pos = input.pos + tam;
                                        input.buf = divstr(input.buf,tam,input.tam-1);
                                        input.tam = input.tam - tam;
                                }
                                if(output.com == 1) {
                                        input.pos = input.pos + tam;
                                        input.tam = input.tam - tam;
                                }
                                //printf("\nSERVER[%d]\n",x);
                                //show_querry(output);
                                send_querry(part[x],output);
                        } else {
                                output.tam = input.tam;
                                if(output.com == 0)
                                        output.buf = input.buf;
                                if(output.com == 1)
                                        output.buf = NULL;
                                //printf("\nSERVER[%d]\n",x);
                                //show_querry(output);
                                send_querry(part[x],output);
                                break;
                        }
                }
        }
        return 0;
}
