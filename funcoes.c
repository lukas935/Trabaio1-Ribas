#include "funcoes.h"

No1 *novono1(char *codigo, int rnn) {
    No1 *novo = malloc(sizeof(No1));
    novo->chave = malloc(6);
    strcpy(novo->chave, codigo);
    novo->endereco = rnn;
    novo->prox = NULL;
    return novo;
}

No2 *novono2(char *titulo) {
    No2 *novo = malloc(sizeof(No2));
    novo->chave = malloc(62);
    strcpy(novo->chave, titulo);
    novo->inicio = NULL;
    novo->prox = NULL;
    return novo;
}

NoCodigo *novoNoCodigo(char *codigo) {
    NoCodigo *novo = malloc(sizeof(NoCodigo));
    //novo->codigo = malloc(6);
    strcpy(novo->codigo, codigo);
    novo->prox = NULL;
    return novo;
}

void listar_filme(FILE *Filme) {
    int rnn = 0;

    fseek(Filme, 0, SEEK_END);
    int total = ftell(Filme) / 192;
    while (rnn < total) {
        imprimir_filme(Filme, rnn);
        rnn++;
    }
}

void buscar_filme(FILE *Filme, indice1 *index1, indice2 *index2) {
    int op;
    do {
        printf("\ndigite 1 para buscar por codigo");
        printf("\ndigite 2 para buscar por titulo");
        scanf("%hd", &op);

        switch (op) {
            case 1:
                buscarChave(Filme, index1);
                break;
            case 2:
                buscar_Titulo(Filme, NULL, index2);
                break;
            default:
                puts("\nopcao invalida");
                break;
        }
    } while (op < 1 || op > 2);
}

void imprimir_filme(FILE *Filme, int rnn) {
    info_filme *f = malloc(sizeof(info_filme));
    fseek(Filme, rnn * 193, SEEK_SET);

    fscanf(Filme, "%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%[^@]@%c@", f->cod_filme, f->titulopt, f->titulo, f->diretor,
           f->ano,
           f->pais, f->nota);

    printf("\nCodigo: %s", f->cod_filme);
    printf("\nTitulo em Portugues: %s", f->titulopt);
    printf("\nTitulo Original: %s", f->titulo);
    printf("\nDiretor: %s", f->diretor);
    printf("\nAno: %s", f->ano);
    printf("\nPais: %s", f->pais);
    printf("\nNota: %c", f->nota);

}

void buscarChave(FILE *Filme, indice1 *index1) {
    char *codigo = malloc(5);
    int rnn;

    printf("\ndigite a chave: ");
    scanf("%5s", codigo);
    limpabuffer();

    rnn = determina_endereco_cod(index1, codigo);
    if (rnn == -1) {
        printf("\nfilme nao encontrado");
        return;
    }

    printf("\n%d", rnn);
    imprimir_filme(Filme, rnn);
}

void buscar_Titulo(FILE *Filme, indice1 *index1, indice2 *index2) {
    char *titulo = malloc(63);
    No2 *no2;
    NoCodigo *inicio;
    char codigo[5];
    int rnn;

    printf("\ndigite o titulo: ");
    scanf("%[^\n]s", titulo);
    limpabuffer();

    no2 = buscano2(index2, titulo);
    inicio = no2->inicio;

    while (inicio != NULL) {
        strcpy(codigo, inicio->codigo);

        rnn = determina_endereco_cod(index1, codigo);

        if (rnn == -1) {
            printf("\n o codigo %s não foi encontrado", codigo);
            return;
        }
        imprimir_filme(Filme, rnn);
        inicio = inicio->prox;
    }
}

void dar_nota(FILE *Filme, indice1 *index1) {
    char *codigo = malloc(6);
    int rnn;
    char *filme = malloc(193);
    char atual;
    char nova;

    printf("\nInsira o codigo do filme: ");
    scanf("%4s", codigo);
    limpabuffer();

    rnn = determina_endereco_cod(index1, codigo);
    if (rnn == -1) {
        printf("\nFilme nao encontrado");
        return;
    }
    fseek(Filme, rnn * 192, SEEK_SET);
    fgets(filme, 192, Filme);
    sscanf(filme, "%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%*[^@]@%c@", &atual);

    printf("\nnota atual: %c", atual);
    printf("\nnova nota: ");
    scanf("%c", &nova);

    if (!isdigit(nova)) {
        puts("\nnota invalida!");
        return;
    }

    int contador = 0;
    char c;

    fseek(Filme, rnn * 193, SEEK_SET);
    while (contador < 6) {
        c = fgetc(Filme);
        if (c == '@')
            contador++;
    }
    fseek(Filme, ftell(Filme), SEEK_SET);
    fputc(nova, Filme);
}

int determina_endereco_cod(indice1 *index, char *codigo) {
    No1 *aux = NULL;

    int left = 0;
    int right = index->tamanho - 1;
    int middle;

    while (left <= right) {
        middle = left + (right - left) / 2;
        aux = index->inicio;
        for (int i = 0; i < middle; i++) {
            aux = aux->prox;
        }
        if (strcmp(aux->chave, codigo) == 0)
            return aux->endereco;
        else if (strcmp(aux->chave, codigo) < 0)
            left = middle + 1;
        else {
            right = middle + 1;
        }
    }
    return -1;
}

void Insere_filme(FILE *Filme, indice1 **index1, indice2 **index2) {
    char nome[15];
    char sobrenome[15];
    info_filme *f = malloc(sizeof(info_filme));
    printf("\nDigite o nome original do filme: ");
    scanf("%[^\n]61s", f->titulo);
    limpabuffer();

    printf("\nDigite o nome em portugues do filme: ");
    scanf("%[^\n]61s", f->titulopt);
    limpabuffer();

    printf("\nDigite o primeiro nome do diretor: ");
    scanf("%14s", nome);
    limpabuffer;

    printf("\nDigite o sobrenome do diretor: ");
    scanf("%14s", sobrenome);
    limpabuffer();

    printf("\nDigite o ano de lancamento do filme: ");
    scanf("%4s", f->ano);
    limpabuffer();

    printf("\nDigite o pais de origem do filme: ");
    scanf("%[^\n]20s", f->pais);
    limpabuffer();

    printf("\nDigite a nota do filme: ");
    scanf("%c", &f->nota);
    limpabuffer();

    sprintf(f->diretor, "%s, %s", sobrenome, nome);
    char aux[4];
    for (int i = 0; i < 3; i++)
        aux[i] = toupper(sobrenome[i]);
    aux[3] = '\0';

    sprintf(f->cod_filme, "%s%c%c", aux, f->ano[2], f->ano[3]);

    for (int i = 0; i < 4; i++) {
        if (!isdigit(f->ano[i])) {
            printf("\nData invalida\n");
            return;
        }
    }
    if (!isdigit(f->nota)) {
        printf("\nNota invalida\n");
        return;
    }

    int rnn = determina_endereco_cod(*index1, f->cod_filme);
    if (rnn != -1) {
        printf("\nEsse Filme ja esta cadastrado\n");
        return;
    }

    if (strcmp(f->titulopt, f->titulo) == 0)
        strcpy(f->titulo, "idem");

    char filme[193];

    sprintf(filme, "%s@%s@%s@%s@%s@%s@%c@", f->cod_filme, f->titulo, f->titulopt, f->diretor, f->ano, f->pais,
            f->nota);

    int tamanho = strlen(filme);
    if (tamanho < 192 + 1) {
        for (int i = tamanho; i < 192; i++)
            filme[i] = '#';
        filme[192] = '\0';
    }

    fseek(Filme, 0, SEEK_END);
    rnn = ftell(Filme) / 192;
    fprintf(Filme, "%s", filme);

    No1 *novo1 = novono1(f->cod_filme, rnn);
    No2 *novo2 = buscano2(*index2, f->titulopt);
    NoCodigo *novoC = novoNoCodigo(f->cod_filme);

    insereNo1(*index1, novo1);

    if (novo2 == NULL) {
        novo2 = novono2(f->titulopt);
        inserecod(novo2, novoC);
        insereNo2(*index2, novo2);
    } else {
        inserecod(novo2, novoC);
    }

    free(f);
}

void inserecod(No2 *pNo2, NoCodigo *pCodigo) {
    if (pNo2->inicio == NULL) {
        pNo2->inicio = pCodigo;
        return;
    }

    NoCodigo *aux = pNo2->inicio;
    NoCodigo *anterior;

    while (aux != NULL && strcmp(pCodigo->codigo, aux->codigo) > 0) {
        anterior = aux;
        aux = aux->prox;
    }
    if (strcmp(pCodigo->codigo, aux->codigo) < 0) {
        pCodigo->prox = aux;
        anterior->prox = pCodigo;
        return;
    }
    anterior->prox = pCodigo;
}


void insereNo1(indice1 *index1, No1 *pNo1) {
    if (index1->inicio == NULL) {
        index1->inicio = pNo1;
        index1->tamanho++;
        return;
    }

    No1 *aux = index1->inicio;
    No1 *anterior;

    while (aux != NULL && strcmp(pNo1->chave, aux->chave) > 0) {
        anterior = aux;
        aux = aux->prox;
    }
    if (strcmp(pNo1->chave, aux->chave) < 0) {
        pNo1->prox = aux;
        anterior->prox = pNo1;
        index1->tamanho++;
        return;
    }
    anterior->prox = pNo1;
    index1->tamanho++;
}

void insereNo2(indice2 *index2, No2 *pNo2) {
    if (index2->inicio == NULL) {
        index2->inicio = pNo2;
        index2->tamanho++;
        return;
    }

    No2 *aux = index2->inicio;
    No2 *anterior;

    while (aux != NULL && strcmp(pNo2->chave, aux->chave) > 0) {
        anterior = aux;
        aux = aux->prox;
    }
    if (strcmp(pNo2->chave, aux->chave) < 0) {
        pNo2->prox = aux;
        anterior->prox = pNo2;
        index2->tamanho++;
        return;
    }
    anterior->prox = pNo2;
    index2->tamanho++;
}

No1 *buscano1(indice1 *index, char codigo[5]) {
    No1 *atual;

    int left = 0;
    int right = index->tamanho - 1;
    int middle;

    while (left <= right) {
        middle = left + (right - left) / 2;

        atual = index->inicio;
        for (int i = 0; i < middle; i++)
            atual = atual->prox;

        if (strcmp(atual->chave, codigo) == 0)
            return atual;
        else if (strcmp(atual->chave, codigo) < 0)
            left = middle + 1;
        else
            right = middle - 1;
    }
    return NULL;
}

No2 *buscano2(indice2 *index, char titulopt[62]) {
    if (index->inicio == NULL)
        return NULL;

    No2 *aux;

    int left = 0;
    int right = index->tamanho;
    int middle;

    while (left <= right) {
        middle = left + (right - left) / 2;
        aux = index->inicio;
        for (int i = 0; i < middle; i++) {
            aux = aux->prox;
        }
        if (strcmp(aux->chave, titulopt) == 0)
            return aux;
        else if (strcmp(aux->chave, titulopt) < 0)
            left = middle + 1;
        else {
            right = middle + 1;
        }
    }
    return NULL;

}

indice1 *novoindice1() {
    indice1 *novo = malloc(sizeof(indice1));
    novo->inicio = NULL;
    novo->tamanho = 0;
    return novo;
}

indice2 *novoindice2() {
    indice2 *novo = malloc(sizeof(indice2));
    novo->inicio = NULL;
    novo->tamanho = 0;
    return novo;
}

indice1 *ler_indice1(FILE *indice) {
    indice1 *novo = novoindice1();
    No1 *aux1 = NULL;

    char codigo[6];
    int rnn;

    fseek(indice, 1, SEEK_SET);
    while (fscanf(indice, "%s@%d@", codigo, &rnn) != EOF) {
        aux1 = novono1(codigo, rnn);
        insereNo1(novo, aux1);
    }
    return novo;
}

indice2 *ler_indice2(FILE *indice) {
    indice2 *novo = novoindice2();
    No2 *aux2 = NULL;

    char titulo[63];
    char codigos[60];
    char codigo[6];

    NoCodigo *auxC = NULL;

    fseek(indice, 1, SEEK_SET);
    while (fscanf(indice, "%[\n]s@%s@", titulo, codigos) != EOF) {
        aux2 = novono2(titulo);
        auxC = NULL;
        while (sscanf(codigos, "%s,", codigo) == 1) {
            auxC = novoNoCodigo(codigo);
            inserecod(aux2, auxC);
        }
        insereNo2(novo, aux2);
    }
    return novo;
}

indice1 *refazer1(FILE *Filme) {
    indice1 *novo = novoindice1();
    int rnn = 0;

    char filme[193];
    char codigo[5];

    No1 *atual = NULL;

    fseek(Filme, 0, SEEK_SET);
    while (fscanf(Filme, "%[^\n]193s", filme) != EOF) {
        if (filme[0] != '*' && filme[1] != '|') {
            sscanf(filme, "%[^@]@", codigo);
            atual = novono1(codigo, rnn);
            insereNo1(novo, atual);
        }
        rnn++;
    }
    return novo;
}

indice2 *refazer2(FILE *Filme) {
    indice2 *novo = novoindice2();

    char filme[193];
    char codigo[5];
    char titulo[62];

    No2 *atual2 = NULL;
    NoCodigo *atualC = NULL;

    fseek(Filme, 0, SEEK_SET);
    while (fscanf(Filme, "%[^\n]193s", filme) != EOF) {
        if (filme[0] != '*' && filme[1] != '|') {
            sscanf(filme, "%[^@]@%[^@]@", codigo, titulo);

            atualC = novoNoCodigo(codigo);
            atual2 = buscano2(novo, titulo);
            if (atual2 == NULL) {
                atual2 = novono2(titulo);
                inserecod(atual2, atualC);
                insereNo2(novo, atual2);
            } else
                inserecod(atual2, atualC);
        }
    }
    return novo;
}

void SaveIndex1(indice1 *prim) {
    FILE *iprimary = fopen("iprimary.idx", "w");

    fputc('1', iprimary);

    No1 *inicioprint = prim->inicio;
    while (inicioprint) {
        fprintf(iprimary, "%s@%d@", inicioprint->chave, inicioprint->endereco);
        inicioprint = inicioprint->prox;
    }

    fseek(iprimary, 0, SEEK_SET);
    fputc('0', iprimary);
    fclose(iprimary);
}


void SaveIndex2(indice2 *secund) {
    FILE *ititle = fopen("data/ititle.idx", "w");

    fputc('1', ititle);

    No2 *inicioprint2 = secund->inicio;
    NoCodigo *inicioprintcod;

    while (inicioprint2) {
        fprintf(ititle, "%s@", inicioprint2->chave);

        inicioprintcod = inicioprint2->inicio;
        while (inicioprintcod) {
            fprintf(ititle, "%s", inicioprintcod->codigo);
            inicioprintcod = inicioprintcod->prox;
        }
        fputc('@', ititle);

        inicioprint2 = inicioprint2->prox;
    }

    fseek(ititle, 0, SEEK_SET);
    fputc('0', ititle);
    fclose(ititle);
}

void freeindex1(indice1 *prim) {
    No1 *aux;
    while (prim->inicio) {
        aux = prim->inicio->prox;
        free(prim->inicio->chave);
        free(prim->inicio);
        prim->inicio = aux;
    }
    free(prim);
}

void freeindex2(indice1 *secund) {
    No2 *atual = secund->inicio;
    while (atual != NULL) {
        No2 *temp = atual;
        atual = atual->prox;
        free(temp->chave);
        freeCodigos(temp->inicio);
        free(temp);
    }
    free(secund);
}

void freeCodigos(NoCodigo *inicio) {
    while (inicio != NULL) {
        NoCodigo *temp = inicio;
        inicio = inicio->prox;
        free(temp->codigo);
        free(temp);
    }
}

void removerNo1(indice1 *index, char codigo[5]) {
    No1 *atual, *ant;

    int left = 0;
    int right = index->tamanho - 1;
    int middle;

    while (left <= right) {
        middle = left + (right - left) / 2;

        atual = index->inicio;
        for (int i = 0; i < middle; i++) {
            ant = atual;
            atual = atual->prox;
        }

        if (strcmp(atual->chave, codigo) == 0) {
            ant->prox = atual->prox;
            free(atual);
            return;
        } else if (strcmp(atual->chave, codigo) < 0)
            left = middle + 1;
        else
            right = middle - 1;
    }
}

void removeNo2(indice2 *index, char titulo[63]) {
    No2 *atual, *anterior;

    int left = 0;
    int right = index->tamanho - 1;
    int middle;

    while (left <= right) {
        middle = left + (right - left) / 2;

        atual = index->inicio;
        for (int i = 0; i < middle; i++) {
            anterior = atual;
            atual = atual->prox;
        }

        if (strcmp(atual->chave, titulo) == 0) {
            anterior->prox = atual->prox;
            free(atual);
            return;
        } else if (strcmp(atual->chave, titulo) < 0)
            left = middle + 1;
        else
            right = middle - 1;
    }
}

void removerNoCodigo(No2 *no, char *codigo) {
    NoCodigo *atual = no->inicio;
    NoCodigo *ant;

    while (strcmp(atual->codigo, codigo) != 0) {
        ant = atual;
        atual = atual->prox;
    }

    ant->prox = atual->prox;
    free(atual);
}

void Remove_filme(FILE *Filme,indice1 **index1, indice2 **index2) {
    char codigo[5];
    char titulo[63];
    int rnn;

    printf("\ninsira a chave do filme a ser deletado: ");
    scanf("%4s",codigo);
    limpabuffer();

    rnn = determina_endereco_cod(*index1,codigo);
    if(rnn == -1){
        printf("\nfilme nao encontrado");
        return;
    }

    string titulo = titulofromcodigo(Filme,rnn);

    fseek(Filme,rnn * 192, SEEK_SET);
    fputs("*|",Filme);

    removeFilmedoindice(*index1, *index2, codigo, titulo);

}

void removeFilmedoindice(indice1 *index1,indice2 *index2,char codigo[5],char titulo[63]){
    removerNo1(index1,codigo);
    No1 *no2 = buscano2(index2, titulo);
    removerNoCodigo(no2,codigo);

    if(no2->inicio == NULL)
        removeNo2(index2,titulo);
}

/*
removerNo1(index1, codigo);
No1 *no1 = buscano1(index1, titulo);
removerNoCodigo(no1, codigo);
 */

void limpabuffer() {
    int rem;
    while ((rem = getchar()) != '\n' && rem != EOF);
}
