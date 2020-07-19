/*
 * Nome: Savio Duarte Fontes - 10737251
 * 
 * Foi utilizado a implementacao do grafo como uma lista
 * Utilizando a biblioteca "lista.h" para tal feito
 */

#include "lista.h"

/* 
 * Função: tempo_dependencia
 * 
 * Parâmetros: 
 * 	Lista** g - grafo em forma de lista
 * 	int v - numero de vertices do grafo
 * 	int *pais - array de numero de pais de cada vertice
 * Retorno: int - tempo para o processo
 * 
 * Resumo: Verifica quais vertices tem 0 pais, coloca eles numa lista.
 * A cada ciclo, os vetices que tem 0 pais, tem seus filhos diminuidos em um no vetor de pais.
 * Se os pais de um novo vertice (filho sendo diminuido) chegarem a zero, ele é adicionado na lista e o processo se repete até a lista estar vazia
 * Retorna o tempo para o processo seja completado por inteiro - caso seja impossivel, é retornado -1
 * 
 */
int tempo_dependencia(Lista** g, int v, int *pais){

	int v_passados = 0; //contador de vertices que já foram analisados
	int contador = 0;   //contador do tempo dos processos

	Lista* l = lista_criar(); //lista complementar que tem todos os vertices com 0 pais
	
	for(int i = 0; i < v; i++){	//insere os vertices que nao tem pais na lista
		if(pais[i] == 0) lista_inserir(l, i);
	}
	
	while(lista_tamanho(l) != 0){	//enquanto a lista não estiver vazia
		
		contador++; //contador de ciclos aumenta
		
		int t = lista_tamanho(l); //numero de vertices a serem analisados no mesmo ciclo
		for(int i = 0; i < t; i++){
			
			int pai = lista_remover_topo(l);//o vertice é removido da lista

			int pai_filhos = lista_tamanho(g[pai]);//pega o numero de filhos que o vertice tem
			for(int j = 0; j < pai_filhos; j++){

				int vertice = lista_remover_topo(g[pai]);//o filho tem o vetor de pais dele diminuido				
				pais[vertice]--;
				
				if(pais[vertice] == 0) lista_inserir(l, vertice);//se o filho ficar com zero pais, ele é inserido na lista
			
			}
			
			v_passados++; //conta o numero vertices que passaram pela lista
		
		}

	}

	lista_apagar(&l); //apaga a lista
	
	if(v_passados == v){//caso o numero de vertices que passaram pela lista for igual ao numero v, retorna o valor de contador
		
		return contador;
	
	} else {//senão é retornado -1

		return -1;

	}
}

int main(void){

	Lista** g;	//Array de Lista*, sendo que cada posicao dele é um vertice do gráfico

	int v;	//numero de vertices
	int e;	//numero de relacoes de dependencia
	
	scanf("%d", &v);
	scanf("%d", &e);
	
	int pais[v];	//Array que conta o numero de pais de cada vertice (processos que sao necessarios fazer antes de fazer o do vertice)

	g = malloc(v*sizeof(Lista*));	//Alocaçao da memoria para o grafo
	
	if(g == NULL){
		printf("Não foi possível alocar memória para o Grafo - Funcao Main | Main.c\n");
		return -1;
	}

	//criacao das listas de cada vertice mais zerando o numero de pais de cada vertice - grafo sendo criado, nenhum vertice tem pai
	for(int i = 0; i < v; i++){
		g[i] = lista_criar();
		pais[i] = 0;
	}

	int v1 = -1, v2 = -1;	//variaveis para coletar as arestas do grafo
	
	for(int i = 0; i < e; i++){
		scanf("%d %d", &v1, &v2);	//leitura da aresta
		lista_inserir(g[v1],v2);	//adicao da aresta no grafo
		pais[v2]++;						//aumenta o numero de pais do vertice "v2", pai é o "v1"
	}
	
	int res = tempo_dependencia(g, v, pais);	//chamada da funcao que calcula o tempo dos processos

	printf("%d\n", res);	//resultado dos processos
	
	//Libera a memoria alocada pelo grafo
	for(int i = 0; i < v; i++){
		lista_apagar(&g[i]);
	}
	free(g);
	
	return 0;

}