#include <stdlib.h>
#include "grafo.h"

/*
 * Struct para o TAD Grafo
 * Contém o ponteiro para a alocação dinamica da matriz de vértices;
 * E também o número de vértices
 */
struct grafo_{
    int** grafo;
    int vertices;
};

/*Função: GrafoCriar
 * Parâmetros: Inteiro v (numero de vertices)
 * Retorno: Endereço alocado dinamicamente para o Grafo
 * Resumo: Recebe o número de vértices e aloca dinamicamente a struct Grafo e devolve seu endereço
 */
Grafo* GrafoCriar(int v){
    
    Grafo* novo = malloc(sizeof(Grafo));
    
    if(novo == NULL){
        printf("Erro - Função GrafoCriar() - impossível alocar memória para o Grafo\n");
   
    } else {
        
        novo->vertices = v;
        novo->grafo = malloc(sizeof(int*)*v);
        if(novo->grafo == NULL){
            printf("Erro - Função GrafoCriar() - impossível alocar memória para as linhas do Grafo\n");
       
        }else{
            for(int i = 0; i < v; i++){
                (novo->grafo)[i] = malloc(sizeof(int)*v);
                if((novo->grafo)[i] == NULL) printf("Erro - Função GrafoCriar() - impossível alocar memória para as colunas do Grafo\n");
            }
        }
    }
    
    return novo;
    
}

/*Função: GrafoInserirAresta
 * Parâmetros: Grafo *g (Endereço do Grafo), int v1 (um vértice da aresta), int v2 (outro vértice da aresta)
 * Retorno: void
 * Resumo: Adiciona uma aresta ao grafo, se possível
 */
void GrafoInserirAresta(Grafo* g, int v1, int v2){
    
    (g->grafo)[v1][v2] = (g->grafo)[v2][v1] = 1;
    
}

/*Função: GrafoRemoverAresta
 * Parâmetros: Grafo *g (Endereço do Grafo), int v1 (um vértice da aresta), int v2 (outro vértice da aresta)
 * Retorno: void
 * Resumo: Remove uma aresta do grafo, se possível
 */
void GrafoRemoverAresta(Grafo* g, int v1, int v2){
    
    (g->grafo)[v1][v2] = (g->grafo)[v2][v1] = 0;
    
}

/*Função: GrafoImprimirMatriz
 * Parâmetros: Grafo *g (Endereço do Grafo)
 * Retorno: void
 * Resumo: Imprime a matriz do Grafo
 */
void GrafoImprimirMatriz(Grafo* g){
    
    for(int i = 0; i < g->vertices; i++){
        for(int j = 0; j < g->vertices; j++){
            printf("%d ", g->grafo[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*Função: GrafoDeletar
 * Parâmetros: Grafo **g (Endereço do endereço do Grafo)
 * Retorno: void
 * Resumo: desaloca toda a memória alocada e deixa vazio o ponteiro para o endereçodo Grafo
 */
void GrafoDeletar(Grafo **g){
    
    if((*g) == NULL){
        return;
    }
    
    for(int i = 0; i < (*g)->vertices; i++){
        free((*g)->grafo[i]);
    }
    free((*g)->grafo);
    free(*g);
    (*g) = NULL;
}
