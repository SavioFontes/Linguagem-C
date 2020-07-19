#include "lista.h"

//Funçao para fazer a inserção da aresta no Grafo
void InserirAresta(Lista **grafo){
    int v1, v2; // Variaveis utilizadas para realaizar funçoes com arestas
    
    scanf("%d %d", &v1, &v2);
    if(!lista_busca(grafo[v1], v2)){ //verifica se a aresta esta no Grafo
        lista_inserir(grafo[v1], v2);
        lista_inserir(grafo[v2], v1);
    }
}

int main(void){
    
    int vertices, arestas_ini; // Variaveis iniciais do programa
    int v1, v2; // Variaveis utilizadas para realaizar funçoes com arestas
    int q, opcao; // Variaveis utilizadas depois que o Grafo foi criado, sendo q a quantidade de opcoes
    
    //Parte Inicial da criação do Grafo
    scanf("%d %d", &vertices, &arestas_ini);
    Lista **grafo = malloc(sizeof(Lista*)*vertices);
    for(int i = 0; i < vertices; i++){ //Criacao da Lista de cada vertice do Grafo
       grafo[i] = lista_criar();
    }
    for(int i = 0; i < arestas_ini; i++){
        InserirAresta(grafo);
    }
    
    scanf("%d", &q); // escaneia o numero de opcoes
    
    /*loop para realizar as opcoes
     * 1: Adicionar Aresta
     * 2: Apagar Aresta
     * 3: Imprimir a Matriz
     */
    for(int i = 0; i < q; i++){
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
                InserirAresta(grafo);
                break;
            
            case 2:
                scanf("%d %d", &v1, &v2);
                lista_remover(grafo[v1], v2);
                lista_remover(grafo[v2], v1);
                break;
                
            case 3:
                for(int i = 0; i < vertices; i++){
                    printf("%d: ", i);
                    lista_imprimir(grafo[i]);
                }
                printf("\n");
                break;
        }
    }
    
    //apaga o Grafo em forma de lista
    for(int i = 0; i < vertices; i++){
        lista_apagar(&grafo[i]);
    }
    free(grafo);

    return 0;
}
