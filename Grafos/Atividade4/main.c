/*
 * Nome: Savio Duarte Fontes - 10737251
 * 
 * Foi utilizado a implementacao do grafo como uma lista
 * Utilizando a biblioteca "grafo.h" para tal feito - cada vertice do grafo tem uma lista dos vertices que ele faz o link
 */

#include "grafo.h"
#define INF 2147483647

/* 
 * Função: prox_vertice(int v, int *passados, int *tem_total)
 * 
 * Parâmetros:
 * 	int v - numero de vertices do Grafo
 * 	int *passados - vetor que diz se o vertice ja foi analisado ou nao
 * 	int *tem_total - vetor dos tempos minimos para chegar em cada vertice
 * 
 * Retorno: int - prox vertice que tem a menor distancia a ser processado pelo algoraitmo de dijkstra para minizar os caminhos
 * 
 * Resumo: Retorna o id do vertice com menor distancia que ainda nao foi processado
 * 
 */
int prox_vertice(int v, int* passados, int *tem_total){
	
	//acha o primeiro vertice que ainda nao foi processado no array
	int a = 0;
	while(passados[a]!=0){
		a++;
	}

	//define ele como o valor de minimo
	int min = a;

	//compara com array se existe algum vertice que tem tempo de distancia menor
	for(int i = min; i < v; i++){
		if(passados[i] == 0 && tem_total[i] < tem_total[min]){
			min = i;
		}
	}

	//poe o vertice com minimo como passado
	passados[min] = 1;

	//retorna o vertice com minima distancia
	return min;
}

/* 
 * Função: relax(int inicio, int fim, int tempo, int valor, int *tem_total, int *valores, int *antecessor)
 * 
 * Parâmetros:
 * 	int inicio - vertice que sai a aresta
 * 	int fim - vertice que chega a aresta
 * 	int tempo -  tempo entre os dois vertices
 * 	int valor - valor entre os dois vertices
 * 	int *tem_total - vetor dos tempos minimos para chegar em cada vertice
 * 	int *valores - vetor com os valores dos caminhos com tempos minimos
 * 	int *antecessor - vetor com os antecessor de cada vertice para o menor caminho
 * 
 * Retorno: void
 * 
 * Resumo: Faz o relaxamento de cada vertice entre os dois vertices. Se os dois vertices tiverem um tempo que somado com o inicial
 * for menor que o atual do final, entao atualiza o tempo do final para essa soma, atualizando o antecessor e a soma dos valores do caminho.
 * 
 */
void relax(int inicio, int fim, int tempo, int valor, int *tem_total, int *valores, int *antecessor){
	if(tem_total[fim] > tem_total[inicio] + tempo){
		
		tem_total[fim] = tem_total[inicio] + tempo;
		valores[fim] = valores[inicio] + valor;
		antecessor[fim] = inicio;
	
	}
}

/* 
 * Função: dijkstra(Grafo **g, int v, int inicio, int fim, int *tem_total, int *valores, int *antecessor)
 * 
 * Parâmetros:
 * 	Grafo **g - grafo para achar o caminho com tempo minimo entre o inicio e fim
 * 	int inicio - vertice inicial do caminho
 * 	int fim - vertice final do caminho
 * 	int *tem_total - vetor dos tempos minimos para chegar em cada vertice
 * 	int *valores - vetor com os valores dos caminhos com tempos minimos
 * 	int *antecessor - vetor com os antecessor de cada vertice para o menor caminho
 * 
 * Retorno: void
 * 
 * Resumo: Algoritmo de dijkstra para o caminho com o menor tempo minimo, se os vertices que forem passados chegarem 
 * ao vertice do fim do caminho, o algortimo é encerrado, ou se o próximo vértice a ser analisado tiver tempo INF, isso quer 
 * dizer que saiu do ciclo feito com o vertice inicio.
 * Enquanto o algoritmo é rodado, é feito o relaxamento dos tempos para ter o tempo minimo.
 */
void dijkstra(Grafo **g, int v, int inicio, int fim, int *tem_total, int *valores, int *antecessor){

	//coloca que o vertice inicio tem tempo 0, ele é o inicio
	tem_total[inicio] = 0;

	//cria um array para verifecar quais vertices foram processados
	int *passados = malloc(sizeof(int)*v);
	for(int i = 0; i < v; i++){
		passados[i] = 0;
	}

	//loop que tem o tamanho de todos os vertices do array para passar por todos os vertices
	for(int i = 0; i < v; i++){

		//pega o proximo vertice a analisar - menor tempo e ainda nao processado - no inicio, u vai receber o vertice inicio
		int u = prox_vertice(v, passados, tem_total);
		
		//caso o vertice processado tiver tempo INF(saiu do ciclo do vertice inicio) ou for igual ao fim, sai do loop, ja que nao tem chance do tempo ficar menor (seria necessario tempos negativos)
		if(tem_total[u] == INF || u == fim) break;

		//variaveis para pegar o vertices da lista que o vertice u conecta
		int vertice, tempo, valor;

		int tam_lista = grafo_tamanho(g[u]);//pega o tamanho da lista que o vertice u tem
		
		for(int j = 0; j < tam_lista; j++){
			
			//pega o vertice que u aponta
			grafo_remover_topo(g[u], &vertice, &tempo, &valor);
			
			//faz o relaxamento do vertice que acabou de ser pego, o qual u aponta para.
			relax(u, vertice, tempo, valor, tem_total, valores, antecessor);
		}
	}

	//libera a memoria alocada para o vetor de vertices passados
	free(passados);
}

/* 
 * Função: printar(int *antecessor, int v)
 * 
 * Parâmetros:
 * 	int *antecessor - vetor que diz os antecessores de cada vertice
 * 	int v - posicao a ser pesquisada o antecessor
 * 
 * Retorno: void
 * 
 * Resumo: acessa recursivamente o vetor de antecessor até chegar no vertice inicial e depois vai saindo das recursões até a primeira chamada fazendo a impressao dos vertices passados
 * 
 */
void printar(int *antecessor, int v){
	if(antecessor[v] == -1){
		printf("%d ", v);
		return;
	}else{
		printar(antecessor, antecessor[v]);
		printf("%d ", v);
	}
}

int main(void){

	Grafo** g;	//Array de Grafo*, sendo que cada posicao dele é um vertice do gráfico

	int v;	//numero de vertices
	int c;	//numero de caminhos
	
	scanf("%d", &v);
	scanf("%d", &c);

	g = malloc(v*sizeof(Grafo*));	//Alocaçao da memoria para o grafo
	
	//Caso nao consiga alocar memoria
	if(g == NULL){
		printf("Não foi possível alocar memória para o Grafo - Funcao Main | Main.c\n");
		return -1;
	}

	//criacao das listas vertices de cada vertice
	for(int i = 0; i < v; i++){
		g[i] = grafo_criar();
	}

	int v1, v2, tempo, valor;	//variaveis para coletar as arestas do grafo
	
	for(int i = 0; i < c; i++){
		scanf("%d %d %d %d", &v1, &v2, &tempo, &valor);	//leitura da aresta
		grafo_inserir(g[v1],v2,tempo,valor);	//adicao da aresta no grafo
	}
	
	//Arrays que sao utilizados no grafo
	int *valores;		//Array com a soma dos valores do ponto inicial do percurso até o vertice com o mesmo id do array
	int *tem_total;	//Array com a soma dos tempos do ponto inicial do percurso até o vertice com o mesmo id do array - priorizando o tempo minimo para o caminho
	int *antecessor;	//Array para dizer qual o antecessor do vertice até a origem, serve para marcar o caminho minimo
	
	//alocacao do espaço dos 3 arrays
	valores = malloc(sizeof(int)*v);
	tem_total = malloc(sizeof(int)*v);
	antecessor = malloc(sizeof(int)*v);

	//preparando os array para o algoritmo de dijkstra
	for(int i = 0; i < v; i++){
		tem_total[i] = INF;		//o tempo para chegar em cada vertice, no inicio, é considerado infinito
		valores[i] = 0;			//a soma dos valores do transporte de um vertice ao outro é considerado nulo
		antecessor[i] = -1;		//é considerado que nenhum vertice é antecessor de nenhum
	}

	int v_inicial, v_final;		//veertices inicial e final do percurso que se pede
	scanf("%d %d", &v_inicial, &v_final);
	
	dijkstra(g, v, v_inicial, v_final, tem_total, valores, antecessor);	//algortimo de dijkstra para o caminho de menor tempo

	printar(antecessor, v_final);		//algoritmo para printar o caminho

	printf("\n%d %d\n", tem_total[v_final], valores[v_final]);	//impressao do tempo e do valor total do caminho

	//libera a memoria alocada pelos 3 arrays
	free(valores);
	free(tem_total);
	free(antecessor);

	//Libera a memoria alocada pelo grafo
	for(int i = 0; i < v; i++){
		grafo_apagar(&g[i]);
	}
	free(g);
	
	return 0;

}



/*colocar como ponteiros os vetores e arruamr eles para passar eles como variavel*/