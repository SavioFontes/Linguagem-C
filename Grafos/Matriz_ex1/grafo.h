#ifndef GRAFO_H
#define GRAFO_H
#include <stdio.h>

typedef struct grafo_ Grafo;

/*Função: GrafoCriar
 * Parâmetros: Inteiro v (numero de vertices)
 * Retorno: Endereço alocado dinamicamente para o Grafo
 * Resumo: Recebe o número de vértices e aloca dinamicamente a struct Grafo e devolve seu endereço
 */
Grafo* GrafoCriar(int v);

/*Função: GrafoInserirAresta
 * Parâmetros: Grafo *g (Endereço do Grafo), int v1 (um vértice da aresta), int v2 (outro vértice da aresta)
 * Retorno: void
 * Resumo: Adiciona uma aresta ao grafo, se possível
 */
void GrafoInserirAresta(Grafo* g, int v1, int v2);

/*Função: GrafoRemoverAresta
 * Parâmetros: Grafo *g (Endereço do Grafo), int v1 (um vértice da aresta), int v2 (outro vértice da aresta)
 * Retorno: void
 * Resumo: Remove uma aresta do grafo, se possível
 */
void GrafoRemoverAresta(Grafo* g, int v1, int v2);

/*Função: GrafoImprimirMatriz
 * Parâmetros: Grafo *g (Endereço do Grafo)
 * Retorno: void
 * Resumo: Imprime a matriz do Grafo
 */
void GrafoImprimirMatriz(Grafo* g);

/*Função: GrafoDeletar
 * Parâmetros: Grafo **g (Endereço do endereço do Grafo)
 * Retorno: void
 * Resumo: desaloca toda a memória alocada e deixa vazio o ponteiro para o endereçodo Grafo
 */
void GrafoDeletar(Grafo **g);

#endif
