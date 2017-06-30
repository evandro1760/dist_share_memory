typedef struct qrry {
        int com;
        int pos;
        char* buf;
        int tam;
} Querry;

Querry parser(char* mens, int len) {
        int i = 0, j = 0, x = 0, y = 0, z = 0, r, w, begin, end, a;
        char *op, *pos, *buf, *size, troca;
        Querry input;
        input.com = -1;
        input.buf = NULL;

        op = (char *) calloc(1,sizeof(char));
        pos = (char *) calloc(1,sizeof(char));
        size = (char *) calloc(1,sizeof(char));
        while (mens[i] != '(') {
                if(mens[i] != ' ') {
                        op = (char *) realloc(op, x + 1);
                        if(mens[i]>='a' && mens[i]<='z') {
                                op[x] = mens[i] - 32;
                        } else {
                                op[x] = mens[i];
                        }
                        x++;
                }
                i++;
        }
        r = abs(strcmp(op,"LE"));
        w = abs(strcmp(op,"ESCREVE"));
        if(w < r) {
                input.com = 0;
        }
        if(w > r) {
                input.com = 1;
        }
        if (w == r) {
                printf("COMANDO INVALIDO[%d]\n",w);
                input.com = -1;
                return input;
        }
        i++;
        while(mens[i] != ',') {
                if(mens[i] != ' ') {
                        pos = (char *) realloc(pos, y + 1);
                        pos[y] = mens[i];
                        y++;
                }
                i++;
        }
        sscanf(pos,"%d",&(input.pos));
        if(input.com == 0) {
                while (mens[i] != '"') {
                        i++;
                }
                begin = i + 1;
        }
        i = len - 1;
        while (mens[i] != ')') {
                i--;
        }
        i--;
        while(mens[i] != ',') {
                if(mens[i] != ' ') {
                        size = (char *) realloc(size, z + 1);
                        size[z] = mens[i];
                        z++;
                }
                i--;
        }
        for (a = 0; a < z/2; a++) {
                troca = size[a];
                size[a] = size[z-1-a];
                size[z-1-a] = troca;
        }
        sscanf(size,"%d",&input.tam);
        if(input.com == 0) {
                while (mens[i] != '"') {
                        i--;
                }
                end = i-1;
                i = 0;
                input.buf = (char *) calloc(end - begin,sizeof(char));
                for (j = begin; j <= end; j++) {
                        input.buf[i] = mens[j];
                        i++;
                }
        }
        return input;
}

void show_querry(Querry input){
        printf("COMANDO: [%d]\n",input.com);
        printf("POSICAO: [%d]\n",input.pos);
        printf("BUFFER: [%s]\n",input.buf);
        printf("TAMANHO: [%d]\n",input.tam);
}

void show(char* buf, int tam){
        int i;
        for (i = 0; i < tam; i++) {
                printf("[%c] ", buf[i]);
        }
        printf("\n");
}

char* inv(char* buf, int tam){
    int i;
    char* fub;

    fub = (char*)malloc(tam*sizeof(char));
    for (i = 0; i < tam; i++) {
        fub[i] = buf[tam-i-1];
    }
    return fub;
}
