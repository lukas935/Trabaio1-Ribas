#include <stdio.h>

#include "funcoes.h"

int main() {
    int option;

    FILE *Filme;
    FILE *Index_Primario;
    FILE *Index_Secundario;
    indice1 *prim;
    indice2 *secund;

    //verificando se existe o arq txt
    Filme = fopen("filmes.txt", "r+");
    if (Filme == NULL)
        Filme = fopen("filmes.txt", "w+");

    //verificando se existe arq de index
    Index_Primario = fopen("iprimary.idx", "r+");
    if (Index_Primario == NULL) {
        Index_Primario = fopen("iprimary.idx", "w+");
        prim = refazer1(Filme);

    } else {
        if (fgetc(Index_Primario) == '0') {
            prim = ler_indice1(Index_Primario);
            //tudo certo
        } else {//tudo errado
            prim = refazer1(Filme);
        }
    }

    Index_Secundario = fopen("ititle.idx", "r+");
    if (Index_Secundario == NULL) {
        Index_Secundario = fopen("ititle.idx", "w+");
        //se estiver vazio
        secund = refazer2(Filme);

    } else {
        if (fgetc(Index_Secundario) == '0') {
            secund = ler_indice2(Index_Secundario);
        }
        else{
            secund = refazer2(Filme);
        }
    }

    do {
        printf("\n\nOpcao 1: Inserir filme\n");
        printf("Opcao 2: Remover filme\n");
        printf("Opcao 3: Dar nota a um filme\n");
        printf("Opcao 4: Buscar filme\n");
        printf("Opcao 5: Listar filmes do catalogo\n");
        printf("Opcao 6: Encerrar o programa\n");
        scanf("%d", &option);
        limpabuffer();

        switch (option) {

            case 1:
                Insere_filme(Filme, &prim, &secund, Index_Primario, Index_Secundario);
                break;

            case 2:
                Remove_filme(Filme, &prim, &secund, Index_Primario, Index_Secundario);
                break;

            case 3:
                dar_nota(Filme, prim);
                break;

            case 4:
                buscar_filme(Filme,prim,secund);
                break;

            case 5:
                listar_filme(Filme);
                break;

            case 6:
                break;
        }

    } while (option != 6);


    fclose(Filme);
    SaveIndex1(Index_Primario, prim);
    fclose(Index_Primario);
    SaveIndex2(Index_Secundario, secund);
    fclose(Index_Secundario);

    freeindex1(prim);
    freeindex2(secund);

    return 0;
}










