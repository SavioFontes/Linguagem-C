#include "functions_main.h"

/*
 * Funcao: registro_transferir(char *source, char *product)
 * Parametros:
 * 	char *source - nome do arquivo para ler os dados;
 * 	char *product - nome do arquivo binario de saida;
 * Retorno: int - 1 para realizado com sucesso | 0 para realizado sem sucesso
 * Resumo:
 * 	Abre o arquivo source e product, le os dados do source e adiciona eles no arquivo product
 */
int registro_transferir(char *source, char *product){

	int funciona;
	FILE* fproduct;					//arquivo de saida
	CABECALHO c = abrir_arquivo(product,2,&fproduct,&funciona);

	FILE* fsource;
	abrir_arquivo(source,9,&fsource,&funciona);

	if(funciona == 1){

		if(!feof(fsource)){							//verifica se o arquivo source nao esta vazio
			
			descarte_primeira_linha(fsource);	//descarta a primeira linha do source, ja que ela eh uma linha informativa dos campos contidos no arquivo
			int contador = 0; 						//contador de quantos registro estao sendo adicionados
			
			char entrada[128];						//string para receber o registro do arquivo source

			while(fgets(entrada,128,fsource) != 0){ 			//loop ate o arquivo fsource acabar e ao mesmo tempo pega o registro do fsource
				
				REGISTRO novo = registro_formatar(entrada);	//formata a string de registro para ficar da forma REGISTRO (struct) e do formato pedido pela implemetacao
				
				registro_inserir(fproduct, novo, 0); 				// insere os registros no *.bin # modo 0 para insercao normal
				
				contador++;												//contador de registros
			}

			c.status = '1';
			c.RRNproxRegistro = contador;
			c.numeroRegistrosInseridos = contador;

			cabecalho_atualizar(fproduct, c); //atualiza o cabecalho depois de todas insercoes
		}

		fclose(fsource);	//fecha o arquivo fsource

	}

	fclose(fproduct); //fecha o arquivo fproduct

	return funciona;
}

/*
 * Funcao: registro_imprimir(char *source)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * Retorno: int - 1 para realizado com sucesso | 0 para realizado sem sucesso
 * Resumo:
 * 	Abre o arquivo com o nome "source" e imprime seus registros
 */
int registro_imprimir(char *source){

	int funciona; //variavel que diz se o codigo foi um sucesso
	FILE *fsource;

	CABECALHO c = abrir_arquivo(source,0,&fsource,&funciona); //funcao para abrir o arquivo
		
	if(funciona == 1){	 //verifica a consistencia do arquivo
		
		if(c.numeroRegistrosInseridos > 0){//verifica se existe registros no arquivo
			
			for(int i = 0; i < c.RRNproxRegistro; i++){	//loop para impressao dos registros
				
				int excluido = 0;
				REGISTRO r = registro_retorna(fsource, &excluido);

				if(!excluido){
					imprimir_formatado(r); //impressao do registro
				}
			}
		
		} else{ //caso nao exista registros no arquivo
			printf("Registro Inexistente.\n");
		}
	
		fclose(fsource); //fecha arquivo

	}

	return funciona;
	
}

/*
 * Funcao: registro_busca(char *source)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * Retorno: int - 1 para realizado com sucesso | 0 para realizado sem sucesso
 * Resumo:
 * 	Abre o arquivo com o nome "source" e imprime somente os registros que batem com a busca especificada
 */
int registro_busca(char *source){

	int funciona; //variavel que diz se o codigo foi um sucesso
	FILE *fsource;

	CABECALHO c = abrir_arquivo(source,0,&fsource,&funciona); //funcao para abrir o arquivo

	int m; //numero de campos de busca
	scanf("%d", &m);
	
	char parametros[2*m][50]; //array com as strings

	for(int i = 0; i < m; i++){//loop para pegar os parametros de busca	
		scanf("%s",parametros[2*i]);				//le o nome do campo
		scan_quote_string(parametros[2*i+1]);		//le o valor do campo
	}

	if(funciona == 1){	 //verifica a consistencia do arquivo
		
		if(c.RRNproxRegistro > 0){//verifica se existe registros no arquivo
			
			int existente = 0;	//variavel que verifica se existem registro que batem com a busca fornecida

			for(int i = 0; i < c.RRNproxRegistro; i++){	//loop para impressao dos registros
				
				int excluido = 0;															//variavel para verifcar se o registro foi ou nao excluido, se foi, ela recebera '1'
				REGISTRO r = registro_retorna(fsource, &excluido); 			//pega o registro
				
				if(!excluido){																//caso o registro nao tenha sido excluido, verifica se o registro esta entre os parametros de busca
					int valido = registro_verifica_busca(r, m, &parametros);	//analisa se o registro bate com o solicitado
					
					if(valido){	//caso o registro esteja conforme a busca
						imprimir_formatado(r);	//impressao do registro
						existente = 1;				//variavel vai para '1' se existir registros
					}
				}
			}

			if(!existente){	//verifica se há ou não registros, caso não, imprime "Registro Inexistente."
				printf("Registro Inexistente.\n");
			}
		
		} else{ //caso nao exista registros no arquivo
			printf("Registro Inexistente.\n");
		}
	
		fclose(fsource); //fecha arquivo

	}

	return funciona;
}

/*
 * Funcao: registro_rrn_imprimir(char *source)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * Retorno: int - 1 para realizado com sucesso | 0 para realizado sem sucesso
 * Resumo:
 * 	Abre o arquivo com o nome "source" e imprime o registro com o rrn espeficicado, se existir.
 */
int registro_rrn_imprimir(char *source){

	int funciona; //variavel que diz se o codigo foi um sucesso
	FILE *fsource;

	CABECALHO c = abrir_arquivo(source,0,&fsource,&funciona); //funcao para abrir o arquivo
	
	int rrn; //numero de campos de busca
	scanf("%d", &rrn);

	if(funciona == 1){
		
		if(c.RRNproxRegistro > 0 && rrn <= c.RRNproxRegistro){//verifica se existe registros no arquivo

			//foi colocado rrn+1, já que foi considerado no run.codes(caso 11) que os registros começam no rrn 0 (no arquivo de dados começa em rrn 1)
			
			fseek(fsource, (rrn+1)*128, SEEK_SET);								//fseek para a posicao do rrn do registro

			int excluido = 0;															//variavel para verifcar se o registro foi ou nao excluido, se foi, ela recebera '1'
			REGISTRO r = registro_retorna(fsource, &excluido); 			//pega o registro
			
			if(!excluido){																//caso o registro nao tenha sido excluido, verifica se o registro esta entre os parametros de busca
				imprimir_formatado(r);
			} else{
				printf("Registro Inexistente.\n");
			}
		
		} else{ //caso nao exista registros no arquivo
			printf("Registro Inexistente.\n");
		}

		fclose(fsource);
	}

	return funciona;
}

/*
 * Funcao: registro_remocao_logica(char *source)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * Retorno: int - 1 para realizado com sucesso | 0 para realizado sem sucesso
 * Resumo:
 * 	Abre o arquivo com o nome "source" e faz a remocao dos registros que batem com os parametros de busca requisitados
 */
int registro_remocao_logica(char *source){

	int funciona; //variavel que diz se o codigo foi um sucesso
	FILE *fsource;

	CABECALHO c = abrir_arquivo(source,1,&fsource,&funciona); //funcao para abrir o arquivo

	if(funciona == 1){	 //verifica a consistencia do arquivo
		
		int n;				//numero de buscas para remocao
		scanf("%d", &n);	

		for(int a = 0; a < n; a++){ //loop para remover por busca

			int m; //numero de campos de busca
			scanf("%d", &m);
			
			char parametros[2*m][50]; //array com as strings

			for(int i = 0; i < m; i++){//loop para pegar os parametros de busca	
				scanf("%s",parametros[2*i]);				//le o nome do campo
				scan_quote_string(parametros[2*i+1]);		//le o valor do campo
			}

			if(c.numeroRegistrosInseridos > 0){//verifica se existe registros inseridos no arquivo
				
				for(int i = 0; i <= c.RRNproxRegistro; i++){	//loop para impressao dos registros
					
					int excluido = 0;															//variavel para verifcar se o registro foi ou nao excluido, se foi, ela recebera '1'
					REGISTRO r = registro_retorna(fsource, &excluido); 			//pega o registro
					
					if(!excluido){																//caso o registro nao tenha sido excluido, verifica se o registro esta entre os parametros de busca
						int valido = registro_verifica_busca(r, m, &parametros);	//analisa se o registro bate com o solicitado
						
						if(valido){	//caso o registro esteja conforme a busca
							
							fseek(fsource, -128, SEEK_CUR);	//fseek para voltar par o inicio do registro a ser removido
							registro_inserir(fsource, r, 1);	//remocao do registro - modo 1 para remocao na funcao registro_inserir
							c.numeroRegistrosInseridos--;
							c.numeroRegistrosRemovidos++;
							
						}
					}
				}
			}

			fseek(fsource,128,SEEK_SET); //Retorna para o inicio dos registros
			
		}

		c.status = '1';
		cabecalho_atualizar(fsource, c);

		fclose(fsource); //fecha arquivo

	}

	return funciona;

}

/*
 * Funcao: registro_inserir_fim(char *source)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * Retorno: int - 1 para realizado com sucesso | 0 para realizado sem sucesso
 * Resumo:
 * 	Abre o arquivo com o nome "source" e faz a adicao dos novos registros no fim.
 */
int registro_inserir_fim(char *source){

	int funciona; //variavel que diz se o codigo foi um sucesso
	FILE *fsource;

	CABECALHO c = abrir_arquivo(source,1,&fsource,&funciona);

	if(funciona == 1){	 //verifica a consistencia do arquivo

		int n;				//numero de registro para inserir
		scanf("%d", &n);	

		for(int a = 0; a < n; a++){ //loop dos registros a serem adicionados
			
			//pega a string do registro a ser adicionado a partir da tela
			char entrada[150];
			scanf(" %[^\n\r]%*c", entrada);

			//formata a string para o formato de REGISTRO
			REGISTRO novo = registro_formatar_6(entrada);

			//insercao para registros no fim do arquivo
			if(ftell(fsource) != (c.RRNproxRegistro+1)*128){ //verifica se o ponteiro esta no fim do arquivo, se nao, ele faz um fseek para o fim do arquivo, se sim, ele nao faz nada
				fseek(fsource, (c.RRNproxRegistro+1)*128, SEEK_SET);
			}
			
			registro_inserir(fsource, novo, 0);	//insere o registro no fim do arquivo no modo normal de insercao(0)
			c.numeroRegistrosInseridos++;			//aumenta o numero de registros inseridos
			c.RRNproxRegistro++;						//aumenta o numero do RRNproxRegistro

		}

		c.status = '1'; //atualiza o status
		cabecalho_atualizar(fsource, c);//reescreve o cabecalho
	
		fclose(fsource); //fecha arquivo

	} 
	
	return funciona;

}

/*
 * Funcao: registro_atualizar(char *source)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * Retorno: int - 1 para realizado com sucesso | 0 para realizado sem sucesso
 * Resumo:
 * 	Abre o arquivo com o nome "source" e faz a atualizacao dos registros que tenham RRN dentro do arquivo modificando os parametros requisitados
 */
int registro_atualizar(char *source){

	int funciona; //variavel que diz se o codigo foi um sucesso
	FILE *fsource;

	CABECALHO c = abrir_arquivo(source,1,&fsource,&funciona);

	if(funciona == 1){	 //verifica a consistencia do arquivo
		
		int n;				//numero de atualizacoes
		scanf("%d", &n);	

		for(int a = 0; a < n; a++){ //loop para atualizar

			int RRN; //RRN de busca
			scanf("%d", &RRN);

			int m; //numero de campos para atualizar
			scanf("%d", &m);
			
			char parametros[2*m][50]; //array com as strings

			for(int i = 0; i < m; i++){//loop para pegar os parametros de busca	
				scanf("%s",parametros[2*i]);				//le o nome do campo
				scan_quote_string(parametros[2*i+1]);		//le o valor do campo
			}

			if(RRN <= c.RRNproxRegistro){	//Caso o RRN do registro exista

				fseek(fsource, (RRN+1)*128, SEEK_SET); //fseek para a posicao

				int excluido = 0; //flag para verificar se o registro está excluido / Utilizacao da funçao registro_retorna
				REGISTRO r = registro_retorna(fsource, &excluido); //pega o registro do RRN pedido

				if(!excluido){
					fseek(fsource, -128, SEEK_CUR); //volta 128 bytes para atualizar o registro

					registro_reescrever(fsource, r, m, &parametros); //chamada da funcao para reescrever os novos dados do registro
					c.numeroRegistrosAtualizados++; //Acrescenta o numeroRegistrosAtualizados
				
				}
			}
			
		}

		c.status = '1';
		cabecalho_atualizar(fsource, c);
	
		fclose(fsource); //fecha arquivo

	}
	
	return funciona;
}