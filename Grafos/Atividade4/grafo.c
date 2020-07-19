#include "grafo.h"

/* Struct no_ é o nó que utilizado para fazer o grafo encadeado */
typedef struct no_ NO;
struct no_{
    int vertice;
    TEMPO tempo;
    VALOR valor;
    NO* proximo;
};

/* Struct grafo_ é a struct que contém os endereços do primeiro e ultimo item mais a quatidade de termos */
struct grafo_{
    NO* inicio;
    NO* fim;
    int tamanho;
};

/* 
 * Função: grafo_criar(void)
 * 
 * Parâmetros: void
 * Retorno: Grafo*
 * 
 * Resumo: Aloca dinamicamente o espaço para criar grafo e coloca as posições incial e
 * final como NULL e o tamanho como 0
 * 
 */
Grafo* grafo_criar(void){
    
    Grafo *novo = malloc(sizeof(Grafo));
    
    if(novo == NULL){
        printf("ERRO - grafo_criar() - Não foi possivel alocar memoria\n");
    } else {
        novo->inicio = NULL;
        novo->fim = NULL;
        novo->tamanho = 0;
    }
    
    return novo;

}

/* 
 * Função: grafo_apagar(Grafo **g)
 * 
 * Parâmetros: Grafo**
 * Retorno: void
 * 
 * Resumo: Libera a alocação dincamicamente primeiro com todos os NO* do grafo e 
 * depois libera a Grafo*
 * 
 */
void grafo_apagar(Grafo **g){
    
    if(*g == NULL){
        printf("ERRO - lista_free - Tentativa de usar free() em NULL\n");
        return;
    }
    
    if((*g)->tamanho > 0){
        
        NO* au1 = (*g)->inicio;
        NO* au2 = NULL;
        for(int i = 0; i < (*g)->tamanho; i++){
            au2 = au1->proximo;
            free(au1);
            au1 = au2;
            
        }
        au1 = NULL;
        au2 = NULL;
    }
    
    free(*g);
    (*g) = NULL;
    
}

/* 
 * Função: grafo_tamanho(Grafo *g)
 * 
 * Parâmetros: Grafo*
 * Retorno: int
 * 
 * Resumo: Retorna o tamanho do Grafo - quantos vertices o vertice em pesquisa aponta
 * 
 */
int grafo_tamanho(Grafo *g){
    return g->tamanho;
}

/* 
 * Função: grafo_inserir(Grafo *g, int vertice, TEMPO tempo, VALOR valor)
 * 
 * Parâmetros:
 *  Grafo* g - vertice que vai apontar para o vertice de parametro
 *  int vertice - vertice que o grafo vai apontar
 *  TEMPO tempo - tempo entre os dois vertices
 *  VALOR valor - valor para ir de um vertice ao outro
 * 
 * Retorno: int - 1 para sucesso - 0 para fracasso
 * 
 * Resumo: Insere um registro na última posição do grafo. Para isso, ela cria um NO*,
 * preenche ele e depois adiciona ele no grafo, depois retorna se a operação foi realizada
 * com sucesso.
 * 
 */
int grafo_inserir(Grafo *g, int vertice, TEMPO tempo, VALOR valor){

    NO *no_novo = malloc(sizeof(NO));
    if(no_novo == NULL){
        printf("ERRO - lista_inserir_registro - Nao foi possivel alocar memoria novo registro");
        return (0);
    }
    
    no_novo->vertice = vertice;
    no_novo->tempo = tempo;
    no_novo->valor = valor;
    no_novo->proximo = NULL;
        
    if(g->inicio == NULL){
        g->inicio = no_novo;
    } else {
        g->fim->proximo = no_novo;
    }

    g->fim = no_novo;
    g->tamanho++;
    
    return (1);
    
}
    
/* 
 * Função: lista_remover_topo(Grafo *g, int* ver, TEMPO* t, VALOR* val)
 * 
 * Parâmetros: 
 *  Grafo* g - grafo que terá o primeiro vertice da lista removido 
 *  int* ver - endereço da variavel que recebera o vertice
 *  TEMPO* t - endereço da variavel que recebera o tempo
 *  VALOR* v - endereço da variavel que recebera o valor
 * 
 * Retorno: int - 0 para fracasso e 1 para sucesso
 * 
 * Resumo: Remove o primeiro nó da lista(Grafo).
 * Depois retorna o se a operacao foi um sucesso.
 * 
 */
int grafo_remover_topo(Grafo *g, int* ver, TEMPO* t, VALOR* val){
    
    if(g->tamanho == 0){
        return 0;
    }
   
    NO* aux1 = g->inicio;
    
    g->inicio = aux1->proximo;

    *ver = aux1->vertice;
    *t = aux1->tempo;
    *val = aux1->valor;
    
    free(aux1);
    aux1 = NULL;
    g->tamanho--;

    return 1;
    
}