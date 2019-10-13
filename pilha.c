#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

typedef struct no NO;
struct no {
    ITEM* item;
    NO *anterior;
};

struct pilha_ {
    NO* topo;
    int tamanho;
};

PILHA* pilha_criar(void){
    
    PILHA* pilha = (PILHA*) malloc(sizeof(PILHA));
    if(pilha!=null){
        pilha->topo = NULL;
        pilha->tamanho = 0;
    }
    return pilha;
    
}
    
void pilha_apagar(PILHA** p){
    
    NO* aux;
    if(!pilha_vazia(*p)){
        
        while((*p)->topo != NULL){
            
            aux = (*p)->topo;
            (*p)->topo = (*p)->topo->anterior;
            item_apagar(&aux->item);
            aux->anterior = NULL;
            free(aux);
            aux = NULL;
            
        }
    }
    free(*p);
    (*p) = NULL;
    
}
   
int pilha_vazia(PILHA *p){
  
    return ((p != NULL) ? p->tamanho == 0 : ERRO);

}

int pilha_tamanho(PILHA *p){
    
    return ((p != NULL) ? p->tamanho : ERRO);
    
}

ITEM* pilha_topo(PILHA *p){
    
    if(!pilha_vazia(p)){
        return (i->topo->item);
    }
    
    return NULL;
    
}

int pilha_empilhar(PILHA *p, ITEM *i){
    
    NO* pnovo = (NO*) malloc(sizeof(NO));
    if(pnovo!=NULL){
        
        pnovo->item = i;
        pnovo->anterior = p->topo;
        p->topo = pnovo;
        p->tamanho++;
       return 1;
   
    }
    
    return ERRO;
    
}

ITEM* pilha_desempilhar(PILHA *p){
    
    if(!pilha_vazia(p)){
        
        NO* pno = p->topo;
        ITEM* item = p->topo->item;
        p->topo=p->topo->anterior;
        pno->anterior = NULL;
        free(pno);
        pno = NULL;
        p->tamanho--;
        return item;
   
    }
    
    return NULL;
    
}
