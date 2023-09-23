#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>

typedef struct no1 {
    char *chave;
    int endereco;
    struct no1 *prox;
} No1;

typedef struct noCodigo {
    char codigo[6];
    struct noCodigo *prox;
} NoCodigo;

typedef struct no2 {
    char *chave;
    NoCodigo *inicio;
    struct no2 *prox;
} No2;

typedef struct {
    int tamanho;
    No1 *inicio;
} indice1;

typedef struct {
    int tamanho;
    No2 *inicio;
} indice2;

typedef struct {
    char cod_filme[6];
    char titulo[62];
    char titulopt[62];
    char diretor[30];
    char ano[5];
    char pais[21];
    char nota;
} info_filme;


int determina_endereco_cod(indice1 *index, char *codigo);

No2 *buscano2(indice2 *index, char titulopt[63]);

void insereNo1(indice1 *index1, No1 *pNo1);

void insereNo2(indice2 *index2, No2 *pNo2);

void inserecod(No2 *pNo2, NoCodigo *pCodigo);

indice1 *ler_indice1(FILE *indice);

indice2 *ler_indice2(FILE *indice);

indice1 *refazer1(FILE *Filme);

indice2 *refazer2(FILE *Filme);

void SaveIndex1(FILE *Index1, indice1 *prim);

void SaveIndex2(FILE *Index2, indice2 *secund);

void Insere_filme(FILE *Filme, indice1 **index1, indice2 **index2, FILE *arqindex1, FILE *arqindex2);

void imprimir_filme(FILE *Filme, int rnn);

void listar_filme(FILE *Filme);

void buscar_filme(FILE *Filme, indice1 *index1, indice2 *index2);

void buscarChave(FILE *Filme, indice1 *index1);

void buscar_Titulo(FILE *Filme, indice1 *index1, indice2 *index2);

void dar_nota(FILE *Filme, indice1 *index1);

void removeFilmedoindice(indice1 *index1, indice2  *index2, char codigo[6], char titulo[63]);

void Remove_filme(FILE *Filme, indice1 **index1, indice2 **index2, FILE *arqindex1, FILE *arqindex2);

void removerNoCodigo(No2 *no, char *codigo);

void freeindex1(indice1 *prim);

void freeindex2(indice2 *secund);

void freeCodigos(NoCodigo *inicio);

void limpabuffer();

indice1 *novoindice1();

#endif //FUNCOES_H
