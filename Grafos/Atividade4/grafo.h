#ifndef GRAFO_H
#define GRAFO_H
#include <stdio.h>
#include <stdlib.h>

typedef int TEMPO;
typedef int VALOR;
typedef struct grafo_ Grafo;

/* Funções que serão utilizadas pelo programa Main */

Grafo* grafo_criar(void);
/*
 * Aloca dinamicamente o espaço para criar Grafo e coloca as posições incial e
 * final como NULL e o tamanho como 0.
 */

void grafo_apagar(Grafo **g);
/*
 * Libera a alocação dincamicamente primeiro com todos os NO* do grafo e 
 * depois libera a Grafo*.
 */

int grafo_tamanho(Grafo *g);
/*
 * Retorna o tamanho do grafo g(lista).
 */

int grafo_inserir(Grafo *l, int vertice, TEMPO tempo, VALOR valor);
/*
 * Insere um registro na última posição do grafo. Para isso, ela cria um NO*,
 * preenche ele e depois adiciona ele no grafo.
 * Retorna se operação realizda com sucesso.
 */

int grafo_remover_topo(Grafo *l, int* ver, TEMPO* t, VALOR* val);
/*
 * Remove o primeiro vertice do grafo (lista).
 * Depois retorna as variaveis do vertice nas variaveis parametros e retorna se a funcao foi um sucesso.
 */

#endif