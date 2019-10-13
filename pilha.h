#ifndef PILHA_H
#define PILHA_H
#define ERRO -32000

typedef int ITEM;
//Utilizado, pois foi considerado ITEM como um inteiro, mas facil implementacao para utilizar TAD ITEM

typedef struct pilha_ PILHA;

PILHA* pilha_criar(void);
void pilha_apagar(PILHA** p);
int pilha_vazia(PILHA *p);
int pilha_tamanho(PILHA *p);
ITEM* pilha_topo(PILHA *p);
int pilha_empilhar(PILHA *p, ITEM *i);
ITEM* pilha_desempilhar(PILHA *p);

#endif
