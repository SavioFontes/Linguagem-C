#include "grafo.h"

//Funçao para fazer a inserção da aresta no Grafo
void InserirAresta(Grafo *matriz){
    int v1, v2; // Variaveis utilizadas para realaizar funçoes com arestas
    
    scanf("%d %d", &v1, &v2);
    GrafoInserirAresta(matriz, v1, v2);
}

int main(void){
    
    int vertices, arestas_ini; // Variaveis iniciais do programa
    int v1, v2; // Variaveis utilizadas para realaizar funçoes com arestas
    int q, opcao; // Variaveis utilizadas depois que o Grafo foi criado, sendo q a quantidade de opcoes
    
    //Parte Inicial da criação do Grafo
    scanf("%d %d", &vertices, &arestas_ini);
    Grafo *matriz = GrafoCriar(vertices);
    for(int i = 0; i < arestas_ini; i++){
        InserirAresta(matriz);
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
                InserirAresta(matriz);
                break;
            
            case 2:
                scanf("%d %d", &v1, &v2);
                GrafoRemoverAresta(matriz, v1, v2);
                break;
                
            case 3:
                GrafoImprimirMatriz(matriz);
                break;
        }
    }
    
    //apaga o Grafo
    GrafoDeletar(&matriz);
    
    return 0;
    
}
