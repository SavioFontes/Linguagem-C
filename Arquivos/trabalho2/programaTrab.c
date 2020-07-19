/*
 * Nomes:
 * 10737251 Savio Duarte Fontes
 * 11218712 Bruno Fernandes Moreira
 **/

#include "functions_main.h"

int main(void){

	int opcao; 						//opcao de iniciar o programa
	char arquivoCSV[30];	//nome do arquivo de leitura
	char arquivoBIN[30];		//nome do arquivo gerado
	scanf("%d", &opcao);			//seleciona a opcao

	if(opcao == 1){ //transfere os registros de arquivos .csv para .bin

		//leitura dos nomes dos arquivos
		scanf(" %[^ \n]", arquivoCSV);
		scanf(" %[^ \n]", arquivoBIN);
		
		//funcao para transferir os registros do *.csv para o *.bin
		if(registro_transferir(arquivoCSV, arquivoBIN)){
			
			binarioNaTela(arquivoBIN);//funcao para imprimir binario
		
		}

	}

	if(opcao == 2){ //imprime os registro de arquivo .bin

		//leitura do nome do arquivo
		scanf(" %[^ \n]", arquivoBIN);
		registro_imprimir(arquivoBIN); //funcao para imprimir

	}

	if(opcao == 3){ //busca e imprime registros com valores e campos determinados

		//leitura do nome do arquivo
		scanf(" %[^ \n]", arquivoBIN);
		registro_busca(arquivoBIN);
	}

	if(opcao == 4){ //imprime o Registro do RRN dado, caso exista registro

		//leitura do nome do arquivo
		scanf(" %[^ \n]", arquivoBIN);
		registro_rrn_imprimir(arquivoBIN);
	}

	if(opcao == 5){ //realiza a remocao dos registro, conforme dados de busca

		//leitura do nome do arquivo
		scanf(" %[^ \n]", arquivoBIN);
		if(registro_remocao_logica(arquivoBIN)){
			
			binarioNaTela(arquivoBIN);//funcao para imprimir binario
		
		}

	}

	if(opcao == 6){ //realiza a insercao de registros, aproveitando espaços vazios
		
		//leitura do nome do arquivo
		scanf(" %[^ \n]", arquivoBIN);
		if(registro_inserir_fim(arquivoBIN)){

			binarioNaTela(arquivoBIN);//funcao para imprimir binario
		
		}

	}

	if(opcao == 7){ //realiza a atualizacao de registro
		
		//leitura do nome do arquivo
		scanf(" %[^ \n]", arquivoBIN);
		if(registro_atualizar(arquivoBIN)){

			binarioNaTela(arquivoBIN);//funcao para imprimir binario
	
		}

	}

	return 0;
}