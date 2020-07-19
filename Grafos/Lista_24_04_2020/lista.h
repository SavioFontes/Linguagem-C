#ifndef LISTA_H
#define LISTA_H
#include <stdio.h>
#include <stdlib.h>

typedef int ITEM;
typedef struct lista_ Lista;

/* Funções que serão utilizadas pelo programa criente */

Lista* lista_criar(void);
/*
 * Aloca dinamicamente o espaço para criar Lista e coloca as posições incial e
 * final como NULL e o tamanho como 0
 */

void lista_apagar(Lista **l);
/*
 * Libera a alocação dincamicamente primeiro com todos os NO* da lista e 
 * depois libera a Lista*
 */

int lista_busca(Lista *l, ITEM item);
/*
 * Verifica se existe tal item na lista
 */

int lista_inserir(Lista *l, ITEM item);
/*
 * Insere um registro na última posição da lista. Para isso, ela cria um NO*,
 * preenche ele e depois adiciona ele na lista.
 * Retorna se operação realizda com sucesso
 */

ITEM lista_remover(Lista *l, ITEM item);
/*
 * Remove um registro dado um nome para busca e remoção e rearranja a lista
 * 
 * Retorna o enderço do ITEM removido
 */

void lista_imprimir(Lista *l);
/*
 * Imprime todos os dados de todos os registros do sistema
 */

#endif
