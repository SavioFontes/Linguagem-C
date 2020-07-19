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
 * Função: lista_busca
 * 
 * Parâmetros: Lista* l, ITEM item
 * Retorno: int
 * 
 * Resumo: Verifica se existe tal item na lista, caso sim, retorna 1, se nao, retorna 0.
 * 
 */
int lista_busca(Lista *l, ITEM item){

    NO *aux = l->inicio;
    while(aux != NULL){
        if(aux->item == item) return 1;
        aux = aux->proximo;
    }

    return 0;
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
 * Função: lista_remover
 * 
 * Parâmetros: Lista* l, ITEM item
 * Retorno: ITEM*
 * 
 * Resumo: Remove um item dado um item para busca e remoção e rearranja a lista.
 * Depois retorna o ITEM dentro do nó.
 * 
 */
ITEM lista_remover(Lista *l, ITEM item){
    
    if(l->tamanho == 0){
        return -1;
    }
   
    
    NO* aux1 = l->inicio;
    NO* aux2 = NULL;
    
    while(aux1 != NULL && (aux1->item) != item){
        aux2 = aux1;
        aux1 = aux1->proximo;
    }
    
    if(aux1 != NULL){
        
        if(aux1 == l->inicio){
            l->inicio = aux1->proximo;
            aux1->proximo = NULL;
        } else {
            aux2->proximo = aux1->proximo;
            aux1->proximo = NULL;
        }
        
        if(aux1 == l->fim){
            l->fim = aux2;
        }
        
        l->tamanho--;
        ITEM i = aux1->item;
        free(aux1);
        aux1 = NULL;
        aux2 = NULL;
        return i;
        
    }
    
    return -1;
    
}

/* 
 * Função: lista_imprimir
 * 
 * Parâmetros: Lista*
 * Retorno: void
 * 
 * Resumo: Imprime todos os itens da lista
 * 
 */
void lista_imprimir(Lista *l){
    
    NO *aux = l->inicio;
    
    for(int i = 0; i < l->tamanho; i++){
        printf("%d ", aux->item);
        aux = aux->proximo;
    }
    printf("\n");
    return;
    
}