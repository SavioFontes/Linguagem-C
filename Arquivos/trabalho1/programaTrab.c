/*
 * Nomes:
 * 10737251 Savio Duarte Fontes
 * 11218712 Bruno Fernandes Moreira
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binarioNaTela.h"

typedef struct{
	int campo1;       //tamanho do campo cidadeMae (tamanho fixo de 4 bytes)
	int campo2;  		//tamanho do campo cidadeBebe (tamanho fixo de 4 bytes)
	char campo3[50];  //cidadeMae (tamanho variável)
	char campo4[50];  //cidadeBebe (tamanho variável)
	int campo5;  		//idNascimento (tamanho fixo de 4 bytes)
	int campo6;  		//idadeMae (tamanho fixo de 4 bytes)
	char campo7[10];  //dataNascimento (tamanho fixo de 10 bytes)
	char campo8;  		//sexoBebe (tamanho fixo de 1 byte)
	char campo9[2];  	//estadoMae (tamanho fixo de 2 bytes)
	char campo10[2]; 	//estadoBebe (tamanho fixo de 2 bytes)
} REGISTRO;

typedef struct{
	char status;
	int RRNproxRegistro;
	int numeroRegistrosInseridos;
	int numeroRegistrosRemovidos;
	int numeroRegistrosAtualizados;
	char lixo[111];	//lixo correspondente aos 111 bytes nao utilizados
} CABECALHO;

/*
 * Funcao: descarte_primeira_linha(FILE *c)
 * Parametros:
 * 	FILE *c - arquivo csv de entrada;
 * Retorno: void
 * Resumo:
 * 	Le a primeira linha que contem os nomes dos campos do csv e faz o descarte deles
 */
void descarte_primeira_linha(FILE *c){

	char aux[15]; 					//array de leitura
	for(int i = 0; i < 8; i++){//loop para ler a primeira linha e descarta-la
		fscanf(c, " %[^,'\n''\r']%*c", aux);
	}

}

/*
 * Funcao: inserir_cabecalho(FILE *b)
 * Parametros:
 * 	FILE *b - arquivo binario a ser gerado;
 * Retorno: void
 * Resumo:
 * 	Cria o cabecalho do arquivo binario de acordo com as especificaoes
 */
void inserir_cabecalho(FILE *b){

	//declara variaveis do cabecalho
	CABECALHO c;
	c.status = '0';
	c.RRNproxRegistro = 0;
	c.numeroRegistrosInseridos = 0;
	c.numeroRegistrosRemovidos = 0;
	c.numeroRegistrosAtualizados = 0;

	//escreve as variaveis do cabecalho no arquivo
	fwrite(&c.status, sizeof(char), 1, b);
	fwrite(&c.RRNproxRegistro, sizeof(int), 1, b);
	fwrite(&c.numeroRegistrosInseridos, sizeof(int), 1, b);
	fwrite(&c.numeroRegistrosRemovidos, sizeof(int), 1, b);
	fwrite(&c.numeroRegistrosRemovidos, sizeof(int), 1, b);
	
	//criacao do lixo para o cabecalho ter o tamanho de um registro
	for(int i = 0; i < 111; i++){
		c.lixo[i] = '$';
	}
	fwrite(c.lixo, sizeof(char), 111, b);
}

/*
 * Funcao: registro_formatar(char *s)
 * Parametros:
 * 	char *s - linha do arquivo *.csv que e um registro para colocar no *bin
 * Retorno: REGISTRO
 * Resumo:
 * 	Le a string de registro e formata ela para colocar no formato da struct REGISTRO
 */
REGISTRO registro_formatar(char *s){
	char aux[50]; 	//variavel auxiliar para leitura de string
	REGISTRO novo; //struct retornada
	char vazio[10] = {'\0','$','$','$','$','$','$','$','$','$'}; //string de vazio para campos de tamanho fixo
	
	
	int i = 0; //variavel para contar a posicao na string s
	
	//leitura campo3
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//Analise conforme pedida no trabalho - se o campo nao houver nada (o campo do *.csv comeca com virgula)
		
		novo.campo1 = 0;						      
		novo.campo3[0] = '\0';
		i++;												//incrementacao no ponteiro

	}else{												//se o campo tiver algo escrito

		novo.campo1 = strlen(aux);					//pega o tamanho do campo e adiciona ele em novo.campo1
		strcpy(novo.campo3, aux);
		i += strlen(aux) + 1;						//incrementacao no ponteiro

	}
	

	//leitura campo4
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	
	if(s[i]==','){										//Analise conforme pedida no trabalho - se o campo nao houver nada (o campo do *.csv comeca com virgula)
		
		novo.campo2 = 0;
		novo.campo4[0] = '\0';
		i++;												//incrementacao no ponteiro

	}else{												//se o campo tiver algo escrito
		
		novo.campo2 = strlen(aux);					//pega o tamanho do campo e adiciona ele em novo.campo2
		strcpy(novo.campo4, aux);
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	
	}

	//leitura campo5
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	novo.campo5 = atoi(aux);						//adiciona no registro
	i += strlen(aux) + 1;							//incrementacao no ponteiro

	//leitura campo6
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//analise se o campo for nul
		novo.campo6 = -1;
		i++;												//incrementacao no ponteiro
	} else{
		novo.campo6 = atoi(aux);
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	}

	//leitura campo7
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//analise se o campo for nul
		for(int a = 0; a < 10; a++){
			novo.campo7[a] = vazio[a];
		}
		i++;												//incrementacao no ponteiro
	} else{
		strncpy(novo.campo7, aux, 10);
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	}

	//leitura campo8
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//analise se o campo for nul
		novo.campo8 = '0';
		i++;												//incrementacao no ponteiro
	} else{
		novo.campo8 = aux[0];
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	}

	//leitura campo9
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//analise se o campo for nul
		novo.campo9[0] = vazio[0];
		novo.campo9[1] = vazio[1];
		i++;												//incrementacao no ponteiro
	} else{
		strncpy(novo.campo9, aux, 2);
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	}
	
	//leitura campo10
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]=='\n' || s[i]=='\r'){					//analise se o campo e vazio ou nao
		novo.campo10[0] = vazio[0];
		novo.campo10[1] = vazio[1];
	} else{
		strncpy(novo.campo10, aux, 2);
	}

	return novo;
}

/*
 * Funcao: registro_inserir(FILE *p, REGISTRO novo)
 * Parametros:
 * 	FILE *p - arquivo product a ser escrito;
 * 	REGISTRO novo - struct com o registro a ser adicionado
 * Retorno: void
 * Resumo:
 * 	adiciona novo ao arquivo product
 */
void registro_inserir(FILE *p, REGISTRO novo){
	
	//logica para espacos variaveis vazios
	int tam_lixo = 97 - novo.campo1 - novo.campo2;
	char lixo[tam_lixo]; //string de $'s para completar o arquivo

	for(int i = 0; i < tam_lixo; i++){
		lixo[i] = '$';
	}

	//escrita do campo 1 ao 4
	fwrite(&novo.campo1, sizeof(int), 1, p);
	fwrite(&novo.campo2, sizeof(int), 1, p);
	fwrite(novo.campo3, sizeof(char), novo.campo1, p);
	fwrite(novo.campo4, sizeof(char), novo.campo2, p);
	
	//escrita de $ nos espacos variaveis vazios
	fwrite(lixo, sizeof(char), tam_lixo, p);

	//escrita dos campos 5 ao 10
	fwrite(&novo.campo5, sizeof(int), 1, p);
	fwrite(&novo.campo6, sizeof(int), 1, p);
	fwrite(novo.campo7, sizeof(char), 10, p);
	fwrite(&novo.campo8, sizeof(char), 1, p);
	fwrite(novo.campo9, sizeof(char), 2, p);
	fwrite(novo.campo10, sizeof(char), 2, p);
}

/*
 * Funcao: atualizar_cabecalho(FILE *p, int cont)
 * Parametros:
 * 	FILE *p - arquivo para atualizar cabecaclho;
 * 	int cont - contador de registros inseridos;
 * Retorno: void
 * Resumo:
 * 	Atualiza o cabecalho, alterando: 
 * 		o status para 1;
 * 		RRNproxRegistro para o proximo (contador + 1);
 * 		numeroRegistrosInseridos para contador;
 * 
 * Obs: Foi optado por fazer essa funcao para fazer somente um fseek,
 * alterando as 3 variaveis juntas, ja que ele eh um trabalho custoso.
 * Caso o programa tivesse outras opcoes nas quais resultaria em remocao
 * ou atualizacao, essa parte seria estruturada de forma diferente, prova-
 * velmente atualizando cada registro separadamente.
 */
void atualizar_cabecalho(FILE *p, int cont){
	
	//Struct CABECALHO
	CABECALHO c;

	//estados das variaveis cabecalho
	c.status = '1';
	c.RRNproxRegistro = cont;
	c.numeroRegistrosInseridos = cont;

	fseek(p, 0, SEEK_SET);
	fwrite(&c.status, sizeof(char), 1, p);
	fwrite(&c.RRNproxRegistro, sizeof(int), 1, p);
	fwrite(&c.numeroRegistrosInseridos, sizeof(int), 1, p);
}

/*
 * Funcao: registro_transferir(char *source, char *product)
 * Parametros:
 * 	char *source - nome do arquivo para ler os dados;
 * 	char *product - nome do arquivo binario de saida;
 * Retorno: void
 * Resumo:
 * 	Abre o arquivo source e product, le os dados do source e adiciona eles no arquivo product
 */
void registro_transferir(char *source, char *product){

	FILE* fproduct;					//arquivo de saida
	fproduct = fopen(product, "wb+");
	inserir_cabecalho(fproduct);	//insere cabecalho do arquivo de saida

	FILE* fsource; 					//arquivo de entrada
	fsource = fopen(source, "r");

	if(fsource!=NULL){								//se o arquivo fsource existir
		
		if(!feof(fsource)){							//verifica se o arquivo source nao esta vazio
			
			descarte_primeira_linha(fsource);	//descarta a primeira linha do source, ja que ela eh uma linha informativa dos campos contidos no arquivo
			int contador = 0; 						//contador de quantos registro estao sendo adicionados
			
			char entrada[128];						//string para receber o registro do arquivo source

			while(fgets(entrada,128,fsource) != 0){ 			//loop ate o arquivo fsource acabar e ao mesmo tempo pega o registro do fsource
				
				REGISTRO novo = registro_formatar(entrada);	//formata a string de registro para ficar da forma REGISTRO (struct) e do formato pedido pela implemetacao
				
				registro_inserir(fproduct, novo); 				// insere os registros no *.bin
				
				contador++;												//contador de registros
			}

			atualizar_cabecalho(fproduct, contador); //atualiza o cabecalho depois de todas insercoes
		}

		fclose(fsource);	//fecha o arquivo fsource
	
	} else{					//caso arquivo source nao exista
		printf("Falha no processamento do arquivo.\n");
	}

	//fecha o arquivo product
	fclose(fproduct);
}

/*
 * Funcao: cabecalho_leitura(FILE *s)
 * Parametros:
 * 	FILE *s - arquivo fonte para ler o cabecalho, o ponteiro do arquivo deve estar no byte offset 0;
 * Retorno: CABECALHO
 * Resumo:
 * 	Le o cabecalho do arquivo e retorna ele na struct em formato de cabecalho
 */
CABECALHO cabecalho_leitura(FILE *s){

	CABECALHO c; //cabecalho a ser retornado

	//leitura das partes do cabecalho
	fread(&c.status,sizeof(char), 1, s);
	fread(&c.RRNproxRegistro,sizeof(int), 1, s);
	fread(&c.numeroRegistrosInseridos,sizeof(int), 1, s);
	fread(&c.numeroRegistrosRemovidos,sizeof(int), 1, s);
	fread(&c.numeroRegistrosAtualizados,sizeof(int), 1, s);
	fread(c.lixo,sizeof(char), 111, s);

	return c;
}

/*
 * Funcao: registro_retorna(FILE *s)
 * Parametros:
 * 	FILE *s - arquivo fonte para ler o registro, o ponteiro do arquivo deve estar no byte offset do registro a ser lido;
 * Retorno: REGISTRO
 * Resumo:
 * 	Le o registro do byte offset definido
 */
REGISTRO registro_retorna(FILE *s){
	
	REGISTRO r;

	//le os campos 1 ao 4
	fread(&r.campo1, sizeof(int), 1, s);
	fread(&r.campo2, sizeof(int), 1, s);
	fread(r.campo3, sizeof(char), r.campo1, s);
	fread(r.campo4, sizeof(char), r.campo2, s);

	//le o espaco variavel nao utilizado
	int tam_lixo = 97 - r.campo1 - r.campo2;
	char lixo[tam_lixo];
	fread(lixo, sizeof(char), tam_lixo, s);
	
	//le os campos 5 ao 10
	fread(&r.campo5, sizeof(int), 1, s);
	fread(&r.campo6, sizeof(int), 1, s);
	fread(r.campo7, sizeof(char), 10, s);
	fread(&r.campo8, sizeof(char), 1, s);
	fread(r.campo9, sizeof(char), 2, s);
	fread(r.campo10, sizeof(char), 2, s);

	return r;
}

/*
 * Funcao: imprimir_formatado(REGISTRO r)
 * Parametros:
 * 	REGISTRO r - registro do qual se pega os dados para fazer a impressao pedida
 * Retorno: void
 * Resumo:
 * 	Pega os dados do REGISTRO e imprime ele na formatacao pedida
 */
void imprimir_formatado(REGISTRO r){

	if(r.campo1 != -1){ //Caso registro nao exista

		char aux[51]; // string auxiliar para impressao

		printf("Nasceu em ");

		//impressao do campo4 - cidadeBebe
		if(r.campo2 == 0){
			printf("-/");
		} else{
			strncpy(aux,r.campo4,r.campo2);
			aux[r.campo2] = '\0';
			printf("%s/", aux);
		}

		//impressa do campo10 - estadoBebe
		if(r.campo10[0] == '\0'){
			printf("-, em ");
		} else{
			strncpy(aux,r.campo10,2);
			aux[2] = '\0';
			printf("%s, em ", aux);
		}

		//impressao do campo7 - dataNascimento
		if(r.campo7[0] == '\0'){
			printf("-, um bebê de sexo ");
		} else{
			strncpy(aux,r.campo7,10);
			aux[10] = '\0';
			printf("%s, um bebê de sexo ", aux);
		}

		//impressao do campo 8 - secoBebe
		if(r.campo8 == '\0'){
			printf("-\n");
		} else if(r.campo8 == '0'){
			printf("IGNORADO.\n");
		} else if(r.campo8 == '1'){
			printf("MASCULINO.\n");
		} else if(r.campo8 == '2'){
			printf("FEMININO.\n");
		}
	}
}

/*
 * Funcao: registro_imprimir(char *source)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * Retorno: void
 * Resumo:
 * 	Abre o arquivo com o nome "source" e imprime seus registros
 */
void registro_imprimir(char *source){

	FILE *fsource;
	fsource = fopen(source,"rb");

	if(fsource != NULL){ //verifica se existe o arquivo
		CABECALHO c = cabecalho_leitura(fsource);

		if(c.status == '1'){	 //verifica a consistencia do arquivo
			
			if(c.numeroRegistrosInseridos > 0){//verifica se existe registros no arquivo
				
				for(int i = 0; i < c.numeroRegistrosInseridos; i++){	//loop para impressao dos registros
					imprimir_formatado(registro_retorna(fsource)); //impressao do registro
				}
			
			} else{ //caso nao exista registros no arquivo
				printf("Registro inexistente.\n");
			}
		
		} else{ //caso arquivo incosistente
			printf("Falha no processamento do arquivo.\n");	
		}

		fclose(fsource); //fecha arquivo
	
	}else{ //caso o arquivo nao exista
		printf("Falha no processamento do arquivo.\n");
	}
	
}

int main(void){

	int opcao; 						//opcao de iniciar o programa
	char arquivoCSV[30];	//nome do arquivo de leitura
	char arquivoBIN[30];		//nome do arquivo gerado
	scanf("%d", &opcao);			//seleciona a opcao

	if(opcao == 1){

		//leitura dos nomes dos arquivos
		scanf(" %[^ \n]", arquivoCSV);
		scanf(" %[^ \n]", arquivoBIN);
		
		registro_transferir(arquivoCSV, arquivoBIN); //funcao para transferir os registros do *.csv para o *.bin
		binarioNaTela(arquivoBIN); //funcao para imprimir binario
	}
	if(opcao == 2){

		//leitura do nome do arquivo
		scanf(" %[^ \n]", arquivoBIN);
		registro_imprimir(arquivoBIN); //funcao para imprimir

	}

	return 0;
}