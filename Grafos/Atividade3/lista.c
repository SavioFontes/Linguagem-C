#include "lista.h"

/* Struct no_ é o nó que utilizado para fazer a lista encadeada */
typedef struct no_ NO;
struct no_{
    ITEM item;
    NO* proximo;
};

/* Struct lista_ é a struct que contém os endereços do primeiro e ultimo item mais a quatidade de termos */
struct lista_{
    NO* inicio;
    NO* fim;
    int tamanho;
};

/* 
 * Função: lista_criar
 * 
 * Parâmetros: void
 * Retorno: Lista*
 * 
 * Resumo: Aloca dinamicamente o espaço para criar Lista e coloca as posições incial e
 * final como NULL e o tamanho como 0
 * 
 */
Lista* lista_criar(void){
    
    Lista *novo = malloc(sizeof(Lista));
    
    if(novo == NULL){
        printf("ERRO - lista_criar() - Não foi possivel alocar memoria\n");
    } else {
        novo->inicio = NULL;
        novo->fim = NULL;
        novo->tamanho = 0;
    }
    
    return novo;

}

/* 
 * Função: lista_apagar
 * 
 * Parâmetros: Lista**
 * Retorno: void
 * 
 * Resumo: Libera a alocação dincamicamente primeiro com todos os NO* da lista e 
 * depois libera a Lista*
 * 
 */
void lista_apagar(Lista **l){
    
    if(*l == NULL){
        printf("ERRO - lista_free - Tentativa de usar free() em NULL\n");
        return;
    }
    
    if((*l)->tamanho > 0){
        
        NO* au1 = (*l)->inicio;
        NO* au2 = NULL;
        for(int i = 0; i < (*l)->tamanho; i++){
            au2 = au1->proximo;
            free(au1);
            au1 = au2;
            
        }
        au1 = NULL;
        au2 = NULL;
    }
    
    free(*l);
    (*l) = NULL;
    
}

/* 
 * Função: lista_tamanho
 * 
 * Parâmetros: Lista*
 * Retorno: int
 * 
 * Resumo: Retorna o tamanho da lista
 * 
 */
int lista_tamanho(Lista *l){
    return l->tamanho;
}

/* 
 * Função: lista_inserir
 * 
 * Parâmetros: Lista* l, ITEM item
 * Retorno: int
 * 
 * Resumo: Insere um registro na última posição da lista. Para isso, ela cria um NO*,
 * preenche ele e depois adiciona ele na lista, depois retorna se a operação foi realizada
 * sucesso.
 * 
 */
int lista_inserir(Lista *l, ITEM item){

    NO *no_novo = malloc(sizeof(NO));
    if(no_novo == NULL){
        printf("ERRO - lista_inserir_registro - Nao foi possivel alocar memoria novo registro");
        return (0);
    }
    
    no_novo->item = item;
    no_novo->proximo = NULL;
    if(no_novo->item == item){
        
        if(l->inicio == NULL){
            l->inicio = no_novo;
        } else {
            l->fim->proximo = no_novo;
        }
        l->fim = no_novo;
        l->tamanho++;
        
        return (1);
    }
    
    return (0);
}
    
/* 
 * Função: lista_remover_topo
 * 
 * Parâmetros: Lista* l, ITEM item
 * Retorno: ITEM*
 * 
 * Resumo: Remove o primeiro nó da lista.
 * Depois retorna o ITEM dentro do nó.
 * 
 */
ITEM lista_remover_topo(Lista *l){
    
    if(l->tamanho == 0){
        return -1;
    }
   
    NO* aux1 = l->inicio;
    
    l->inicio = aux1->proximo;

    ITEM i = aux1->item;
    free(aux1);
    aux1 = NULL;
    l->tamanho--;

    return i;
    
}