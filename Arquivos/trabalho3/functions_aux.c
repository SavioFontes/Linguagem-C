#include "functions_aux.h"
#include "functions_main.h"

/*
 * Funcao: abrir_arquivo(char *source, int modo, FILE **abrir, int* funciona)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * 	int modo - modo de abertura do arquivo: 0 - modo rb | 1 - modo rb+
 * 	FILE **abrir - ponteiro para o arquivo a ser aberto
 * 	int *funciona - flag para verificacao do sucesso de abertura do arquivo
 * 
 * Retorno: CABECALHO - cabecalho do arquivo aberto
 * Resumo:
 * 	Abre o arquivo com o nome "source" e pega seu cabecalho.
 * 	Verifica o status do arquivo, caso o modo seje rb+, o status ja eh atualizado para 0.
 */
CABECALHO abrir_arquivo(char *source, int modo, FILE **abrir, int* funciona){

	if(modo == 0) *abrir = fopen(source, "rb"); //abre o arquivo modo rb
	if(modo == 1) *abrir = fopen(source, "rb+");//abre o arquivo modo rb+
	if(modo == 2) *abrir = fopen(source, "wb+");//abre o arquivo modo wb+ 
	if(modo == 9) *abrir = fopen(source, "r"); //abre o arquivo modo r
 
	CABECALHO c; //Cabecalho do arquivo

	if(*abrir == NULL){ //caso arquivo inexistente
		
		*funciona = 0; //flag que verifica se o arquivo se esta apto para operacoes

		if(modo != 9){
			
			printf("Falha no processamento do arquivo.\n");
			
		}else{
			
			printf("Falha no carregamento do arquivo.\n");
		
		}

	} else{ //caso arquivo existente

		if(modo == 2){
			c.status = '0';							//
			c.RRNproxRegistro = 0;					//
			c.numeroRegistrosInseridos = 0;		//recebendo parametros conforme a iniciacao do arquivo
			c.numeroRegistrosRemovidos = 0;		//
			c.numeroRegistrosAtualizados = 0;	//

			cabecalho_atualizar(*abrir, c);		//insere cabecalho do arquivo binario
			*funciona = 1;
			return c;
		}

		if(modo == 9){
			*funciona = 1;
			return c;
		}

		c = cabecalho_leitura(*abrir); //le o cabecalho do arquivo
	
		if(c.status == '0'){ //caso o arquivo esteja com status de invalido

			printf("Falha no processamento do arquivo.\n");
			fclose(*abrir); //fecha o arquivo
			*abrir = NULL;
			*funciona = 0; //flag que verifica se o arquivo se esta apto para operacoes
		
		} else{ //caso arquivo esteje valido
		
			*funciona = 1; //flag que verifica se o arquivo se esta apto para operacoes
		
			if(modo == 1){ //caso modo seja rb+, já deixa o status sendo '0'
				
				c.status = '0';
				cabecalho_atualizar(*abrir,c); //atualiza o cabecalho
			
			}
		}
	}

	return c; //retorna o cabecalho da funcao
}

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
	
	//leitura cidadeMae
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//Analise conforme pedida no trabalho - se o campo nao houver nada (o campo do *.csv comeca com virgula)
		
		novo.tamCidadeMae = 0;						      
		novo.cidadeMae[0] = '\0';
		i++;												//incrementacao no ponteiro

	}else{												//se o campo tiver algo escrito

		novo.tamCidadeMae = strlen(aux);					//pega o tamanho do campo e adiciona ele em novo.tamCidadeMae
		strcpy(novo.cidadeMae, aux);
		i += strlen(aux) + 1;						//incrementacao no ponteiro

	}
	

	//leitura cidadeBebe
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	
	if(s[i]==','){										//Analise conforme pedida no trabalho - se o campo nao houver nada (o campo do *.csv comeca com virgula)
		
		novo.tamCidadeBebe = 0;
		novo.cidadeBebe[0] = '\0';
		i++;												//incrementacao no ponteiro

	}else{												//se o campo tiver algo escrito
		
		novo.tamCidadeBebe = strlen(aux);					//pega o tamanho do campo e adiciona ele em novo.tamCidadeBebe
		strcpy(novo.cidadeBebe, aux);
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	
	}

	//leitura idNascimento
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	novo.idNascimento = atoi(aux);						//adiciona no registro
	i += strlen(aux) + 1;							//incrementacao no ponteiro

	//leitura idadeBebe
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//analise se o campo for nul
		novo.idadeBebe = -1;
		i++;												//incrementacao no ponteiro
	} else{
		novo.idadeBebe = atoi(aux);
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	}

	//leitura dataNascimento
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//analise se o campo for nul
		for(int a = 0; a < 10; a++){
			novo.dataNascimento[a] = vazio[a];
		}
		i++;												//incrementacao no ponteiro
	} else{
		strncpy(novo.dataNascimento, aux, 10);
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	}

	//leitura sexoBebe
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//analise se o campo for nul
		novo.sexoBebe = '0';
		i++;												//incrementacao no ponteiro
	} else{
		novo.sexoBebe = aux[0];
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	}

	//leitura estadoMae
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]==','){										//analise se o campo for nul
		novo.estadoMae[0] = vazio[0];
		novo.estadoMae[1] = vazio[1];
		i++;												//incrementacao no ponteiro
	} else{
		strncpy(novo.estadoMae, aux, 2);
		i += strlen(aux) + 1;						//incrementacao no ponteiro
	}
	
	//leitura estadoBebe
	sscanf(s + i, " %[^,\n\r]%*c", aux);		//le o campo
	if(s[i]=='\n' || s[i]=='\r'){					//analise se o campo e vazio ou nao
		novo.estadoBebe[0] = vazio[0];
		novo.estadoBebe[1] = vazio[1];
	} else{
		strncpy(novo.estadoBebe, aux, 2);
	}

	return novo;
}

/*
 * Funcao: registro_inserir(FILE *p, REGISTRO novo, int modo)
 * Parametros:
 * 	FILE *p - arquivo product a ser escrito;
 * 	REGISTRO novo - struct com o registro a ser adicionado
 * 	int modo - 0 para insercao normal | 1 para remocao logica | 2 para atualizar registros
 *
 * Retorno: void
 * Resumo:
 * 	Adiciona o registro novo ao arquivo binario.
 * 
 */
void registro_inserir(FILE *p, REGISTRO novo, int modo){
	
	int tam_cidadeMae = novo.tamCidadeMae; //tamanho do cidadeMae
	if(modo == 1){
		novo.tamCidadeMae = -1;
	}
	
	//escrita do campo 1 ao 4
	fwrite(&novo.tamCidadeMae, sizeof(int), 1, p);
	fwrite(&novo.tamCidadeBebe, sizeof(int), 1, p);
	fwrite(novo.cidadeMae, sizeof(char), tam_cidadeMae, p);
	fwrite(novo.cidadeBebe, sizeof(char), novo.tamCidadeBebe, p);
	
	//logica para espacos variaveis vazios
	int tam_lixo = 97 - tam_cidadeMae - novo.tamCidadeBebe;

	//verificaçao do numero de $'s para atualizacao de registro
	char lixo[97]; //string de lixo para completar o arquivo
	
	if(modo == 2){ //caso modo de atualizacao
		
		fread(lixo, sizeof(char), tam_lixo, p); // leitura dos bytes não utilizados de espaço variavel
	
	} else{ //caso outros modos

		for(int i = 0; i < tam_lixo; i++){
			lixo[i] = '$';
		}

		//escrita de $ nos espacos variaveis vazios
		fwrite(lixo, sizeof(char), tam_lixo, p);

	}

	//escrita dos campos 5 ao 10
	fwrite(&novo.idNascimento, sizeof(int), 1, p);
	fwrite(&novo.idadeBebe, sizeof(int), 1, p);
	fwrite(novo.dataNascimento, sizeof(char), 10, p);
	fwrite(&novo.sexoBebe, sizeof(char), 1, p);
	fwrite(novo.estadoMae, sizeof(char), 2, p);
	fwrite(novo.estadoBebe, sizeof(char), 2, p);
}

/*
 * Funcao: cabecalho_atualizar(FILE *p, CABECALHO c)
 * Parametros:
 * 	FILE *p - arquivo para atualizar cabecaclho;
 * 	CABECALHO c - cabecalho a ser atualizado no arquivo
 * Retorno: void
 * Resumo:
 * 	Atualiza o cabecalho, passando a struct "CABECALHO" como parametro a ser colocada na posicao do cabecalho
 */
void cabecalho_atualizar(FILE *p, CABECALHO c){
	
	//atualiza o cabecalho do binario 

	fseek(p, 0, SEEK_SET);													//fseek para o inicio do arquivo
	fwrite(&c.status, sizeof(char), 1, p);								//carrega o status
	fwrite(&c.RRNproxRegistro, sizeof(int), 1, p);					//carrega o RRNproxRegistro
	fwrite(&c.numeroRegistrosInseridos, sizeof(int), 1, p);		//carrega o numeroRegistrosInseridos
	fwrite(&c.numeroRegistrosRemovidos, sizeof(int), 1, p);		//carrega o numeroRegistrosRemovidos
	fwrite(&c.numeroRegistrosAtualizados, sizeof(int), 1, p);	//carrega o numeroRegistrosAtualizados

	//criacao do lixo para o cabecalho ter o tamanho de um registro
	for(int i = 0; i < 111; i++){
		c.lixo[i] = '$';
	}
	fwrite(c.lixo, sizeof(char), 111, p);
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
 * Funcao: registro_retorna(FILE *s, int *excluido)
 * Parametros:
 * 	FILE *s - arquivo fonte para ler o registro, o ponteiro do arquivo deve estar no byte offset do registro a ser lido;
 *		int *exluido - recebe 1 se o registro estiver exluido
 * Retorno: REGISTRO
 * Resumo:
 * 	Le o registro do byte offset definido
 */
REGISTRO registro_retorna(FILE *s, int *excluido){
	
	REGISTRO r;

	//le os campos 1 ao 4
	fread(&r.tamCidadeMae, sizeof(int), 1, s);

	if(r.tamCidadeMae == -1){  //caso o registro esteja excluido, a variavel excluido recebe 1, faz a leitura do restante do registro e retorna
		*excluido = 1;
		char r_excluido[124];
		fread(r_excluido, sizeof(char), 124, s);
		return r;
	}

	fread(&r.tamCidadeBebe, sizeof(int), 1, s);
	fread(r.cidadeMae, sizeof(char), r.tamCidadeMae, s);
	fread(r.cidadeBebe, sizeof(char), r.tamCidadeBebe, s);

	//le o espaco variavel nao utilizado
	int tam_lixo = 97 - r.tamCidadeMae - r.tamCidadeBebe;
	char lixo[tam_lixo];
	fread(lixo, sizeof(char), tam_lixo, s);
	
	//le os campos 5 ao 10
	fread(&r.idNascimento, sizeof(int), 1, s);
	fread(&r.idadeBebe, sizeof(int), 1, s);
	fread(r.dataNascimento, sizeof(char), 10, s);
	fread(&r.sexoBebe, sizeof(char), 1, s);
	fread(r.estadoMae, sizeof(char), 2, s);
	fread(r.estadoBebe, sizeof(char), 2, s);

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

	char aux[51]; // string auxiliar para impressao

	printf("Nasceu em ");

	//impressao do cidadeBebe - cidadeBebe
	if(r.tamCidadeBebe == 0){
		printf("-/");
	} else{
		strncpy(aux,r.cidadeBebe,r.tamCidadeBebe);
		aux[r.tamCidadeBebe] = '\0';
		printf("%s/", aux);
	}

	//impressa do estadoBebe - estadoBebe
	if(r.estadoBebe[0] == '\0'){
		printf("-, em ");
	} else{
		strncpy(aux,r.estadoBebe,2);
		aux[2] = '\0';
		printf("%s, em ", aux);
	}

	//impressao do dataNascimento - dataNascimento
	if(r.dataNascimento[0] == '\0'){
		printf("-, um bebê de sexo ");
	} else{
		strncpy(aux,r.dataNascimento,10);
		aux[10] = '\0';
		printf("%s, um bebê de sexo ", aux);
	}

	//impressao do campo 8 - secoBebe
	if(r.sexoBebe == '\0'){
		printf("-\n");
	} else if(r.sexoBebe == '0'){
		printf("IGNORADO.\n");
	} else if(r.sexoBebe == '1'){
		printf("MASCULINO.\n");
	} else if(r.sexoBebe == '2'){
		printf("FEMININO.\n");
	}
	
}

/*
 * Funcao: registro_verifica_busca(REGISTRO r, int m, char parametros[1][2*m][50])
 * Parametros:
 * 	REGISTRO r - registro a ser comparado com os parametros de busca
 * 	int m - numero de parametros de busca
 * 	char parametros[1][2*m][50] - tabela de parametros de busca
 * Retorno: void
 * Resumo:
 * 	Recebe a struct "REGISTRO" e verifica se o registro se enquadra na busca. Se sim, retorna 1, caso contrário, 0.
 */
int registro_verifica_busca(REGISTRO r, int m, char parametros[1][2*m][50]){
	
	int valido = 0; //variavel para verificar a validade da busca

	for(int i = 0; i < m; i++){	//loop para os comparacao dos campos do registro

		//caso o parametro de busca for cidadeMae, se o campo do registro for igual ao de busca, valido++
		if(0 == strcmp((*parametros)[2*i],"cidadeMae")){
			if(r.tamCidadeMae != 0){
				if(0 == strncmp((*parametros)[2*i+1],r.cidadeMae,r.tamCidadeMae)){
					valido++;
				}
			}
		}

		//caso o parametro de busca for cidadeBebe, se o campo do registro for igual ao de busca, valido++
		if(0 == strcmp((*parametros)[2*i],"cidadeBebe")){
			if(r.tamCidadeBebe != 0){
				if(0 == strncmp((*parametros)[2*i+1],r.cidadeBebe,r.tamCidadeBebe)){
					valido++;
				}
			}
		}

		//caso o parametro de busca for idNascimento, se o campo do registro for igual ao de busca, valido++
		if(0 == strcmp((*parametros)[2*i],"idNascimento")){
			if(atoi((*parametros)[2*i+1]) == r.idNascimento){
				valido++;
			}
		}

		//caso o parametro de busca for idadeMae, se o campo do registro for igual ao de busca, valido++
		if(0 == strcmp((*parametros)[2*i],"idadeMae")){
			if(atoi((*parametros)[2*i+1]) == r.idadeBebe){
				valido++;
			}
		}

		//caso o parametro de busca for dataNascimento, se o campo do registro for igual ao de busca, valido++
		if(0 == strcmp((*parametros)[2*i],"dataNascimento")){
			if(strlen((*parametros)[2*i+1]) != 0){
				if(0 == strncmp((*parametros)[2*i+1],r.dataNascimento,10)){
					valido++;
				}
			}
		}

		//caso o parametro de busca for sexoBebe, se o campo do registro for igual ao de busca, valido++
		if(0 == strcmp((*parametros)[2*i],"sexoBebe")){
			if((*parametros)[2*i+1][0] == r.sexoBebe){
				valido++;
			}
		}

		//caso o parametro de busca for estadoMae, se o campo do registro for igual ao de busca, valido++
		if(0 == strcmp((*parametros)[2*i],"estadoMae")){
			if(strlen((*parametros)[2*i+1]) != 0){
				if(0 == strncmp((*parametros)[2*i+1],r.estadoMae,2)){
					valido++;
				}
			}
		}

		//caso o parametro de busca for estadoBebe, se o campo do registro for igual ao de busca, valido++
		if(0 == strcmp((*parametros)[2*i],"estadoBebe")){
			if(strlen((*parametros)[2*i+1]) != 0){
				if(0 == strncmp((*parametros)[2*i+1],r.estadoBebe,2)){
					valido++;
				}
			}
		}

	}

	if(valido == m) return 1;
	return 0;
}

/*
 * Funcao: registro_formatar_6(char *s)
 * Parametros:
 * 	char *s - linha de entrada que contem o registro a ser formatado para o formato do arquivo .bin
 * Retorno: REGISTRO
 * Resumo:
 * 	Le a string de registro e formata ela para colocar no formato da struct REGISTRO
 */
REGISTRO registro_formatar_6(char *s){

	char aux[50]; 	//variavel auxiliar para leitura de string
	REGISTRO novo; //struct retornada
	char vazio[10] = {'\0','$','$','$','$','$','$','$','$','$'}; //string de vazio para campos de tamanho fixo
	
	
	int i = 0; //variavel para contar a posicao na string s


	//leitura cidadeMae
	if(s[i]=='"'){ //caso o campo nao seje nulo
		i++;
		
		novo.tamCidadeMae = 0;	//copia dos caracteres até as aspas duplas
		while(s[i]!='"'){
			novo.cidadeMae[novo.tamCidadeMae] = s[i];
			i++;
			novo.tamCidadeMae++;
		}
		
		i+=2; //i vai para o primeiro caracter do proximo campo

	} else{ //caso o campo seje NULO

		novo.tamCidadeMae = 0;
		novo.cidadeMae[0] = '\0';
		i+=5; //i vai para o proximo caracter do proximo campo

	}

	//leitura cidadeBebe
	if(s[i]=='"'){ //caso o campo nao seje nulo
		i++;
		
		novo.tamCidadeBebe = 0;	//copia dos caracteres até as aspas duplas
		while(s[i]!='"'){
			novo.cidadeBebe[novo.tamCidadeBebe] = s[i];
			i++;
			novo.tamCidadeBebe++;
		}
		
		i+=2; //i vai para o primeiro caracter do proximo campo

	} else{ //caso o campo seje NULO

		novo.tamCidadeBebe = 0;
		novo.cidadeBebe[0] = '\0';
		i+=5; //i vai para o proximo caracter do proximo campo

	}
	
	//leitura idNascimento
	if(s[i]=='N'){ //caso o campo seje nulo
		
		novo.idNascimento = -1;
		i+=5;
		
	} else{ //caso o campo nao seje NULO

		sscanf(s + i, " %[^ ]%*c", aux);			//leitura do campo
		i += strlen(aux) + 1;						//incremento do ponteiro
		novo.idNascimento = atoi(aux);					//adiciona no registro

	}
	

	//leitura idadeBebe
	if(s[i]=='N'){ //caso o campo seje nulo
		
		novo.idadeBebe = -1;
		i+=5;
		
	} else{ //caso o campo nao seje NULO

		sscanf(s + i, " %[^ ]%*c", aux);			//leitura do campo
		i += strlen(aux) + 1;						//incremento do ponteiro
		novo.idadeBebe = atoi(aux);					//adiciona no registro

	}

	//leitura dataNascimento
	if(s[i]=='N'){ //caso o campo seje nulo
		
		for(int a = 0; a < 10; a++){				//loop para preencher o registro como vazio
			novo.dataNascimento[a] = vazio[a];
		}
		i+=5;
		
	} else{ //caso o campo nao seje NULO

		i++;												//incremento do ponteiro
		for(int a = 0; a < 10; a++, i++){		//loop para pegar a data
			novo.dataNascimento[a] = s[i];
		}

		i+=2;												//incremento do ponteiro

	}

	//leitura sexoBebe
	if(s[i]=='N'){ //caso o campo seje nulo
		
		novo.sexoBebe = '0';
		i+=5;
		
	} else{ //caso o campo nao seje NULO

		i++;												//incremento do ponteiro
		novo.sexoBebe = s[i];

		i+=3;												//incremento do ponteiro

	}

	//leitura estadoMae
	if(s[i]=='N'){ //caso o campo seje nulo
		
		for(int a = 0; a < 2; a++){				//loop para preencher o registro como vazio
			novo.estadoMae[a] = vazio[a];
		}
		i+=5;
		
	} else{ //caso o campo nao seje NULO

		i++;												//incremento do ponteiro
		for(int a = 0; a < 2; a++, i++){		//loop para pegar a data
			novo.estadoMae[a] = s[i];
		}

		i+=2;												//incremento do ponteiro

	}
	
	//leitura estadoBebe
	if(s[i]=='N'){ //caso o campo seje nulo
		
		for(int a = 0; a < 2; a++){				//loop para preencher o registro como vazio
			novo.estadoBebe[a] = vazio[a];
		}
		i+=5;
		
	} else{ //caso o campo nao seje NULO

		i++;												//incremento do ponteiro
		for(int a = 0; a < 2; a++, i++){		//loop para pegar a data
			novo.estadoBebe[a] = s[i];
		}

		i+=2;												//incremento do ponteiro

	}

	return novo;

}

/* 
 * Funcao: registro_reescrever(FILE *f, REGISTRO r, int m, char parametros[1][2*m][50])
 * Parametros:
 * 	FILE *f - arquivo binario para reescrever o registro (Ponteiro deve estar na posicao do registro a ser modificado)
 * 	REGISTRO r - Registro a ter campos atualizados
 * 	int m - numero de campos a serem atualizados
 * 	char parametros[1][2*m][50] - tabela de parametros a serem atualizados
 * Retorno: void
 * Resumo:
 * 	Faz um loop, reescrevendo os campos necessarios de REGISTRO r e depois faz a chamada da funcao registro_inserir()
 */
void registro_reescrever(FILE *f, REGISTRO r, int m, char parametros[1][2*m][50]){

	for(int i = 0; i < m; i++){

		//caso o parametro for cidadeMae
		if(0 == strcmp((*parametros)[2*i],"cidadeMae")){
			
			if(0 == strcmp("",(*parametros)[2*i+1])){ //caso o campo for NULO
				
				r.tamCidadeMae = 0;
				r.cidadeMae[0] = '\0';
			
			} else{ //caso nao for NULO
				
				r.tamCidadeMae = strlen((*parametros)[2*i+1]);
				for(int j = 0; j < r.tamCidadeMae; j++){
					r.cidadeMae[j] = (*parametros)[2*i+1][j];
				}

			}

		}

		//caso o parametro for cidadeBebe
		if(0 == strcmp((*parametros)[2*i],"cidadeBebe")){
			
			if(0 == strcmp("",(*parametros)[2*i+1])){ //caso o campo for nulo
				
				r.tamCidadeBebe = 0;
				r.cidadeBebe[0] = '\0';
			
			} else{ //caso nao for NULO

				r.tamCidadeBebe = strlen((*parametros)[2*i+1]);
				for(int j = 0; j < r.tamCidadeBebe; j++){
					r.cidadeBebe[j] = (*parametros)[2*i+1][j];
				}

			}
		}

		//caso o parametro for idNascimento
		if(0 == strcmp((*parametros)[2*i],"idNascimento")){
			
			if(0 == strcmp("",(*parametros)[2*i+1])){ //caso o campo for nulo
				
				r.idNascimento = -1;
			
			} else{ //caso nao for nulo

				r.idNascimento = atoi((*parametros)[2*i+1]);
			
			}
			
		}

		//caso o parametro for idadeMae
		if(0 == strcmp((*parametros)[2*i],"idadeMae")){
			
			if(0 == strcmp("",(*parametros)[2*i+1])){ //caso o campo for nulo
				
				r.idadeBebe = -1;
			
			} else{ //caso nao for nulo

				r.idadeBebe = atoi((*parametros)[2*i+1]);
			
			}
		}

		//caso o parametro for dataNascimento
		if(0 == strcmp((*parametros)[2*i],"dataNascimento")){

			if(0 == strcmp((*parametros)[2*i+1],"")){ //caso for nulo
				
				r.dataNascimento[0] = '\0';
				for(int j = 1; j < 10; j++){
					r.dataNascimento[j] = '$';
				}

			} else { //caso nao for nulo

				for(int j = 0; j < 10; j++){
					r.dataNascimento[j] = (*parametros)[2*i+1][j];
				}

			}
		}

		//caso o parametro for sexoBebe
		if(0 == strcmp((*parametros)[2*i],"sexoBebe")){
			if(0 == strcmp((*parametros)[2*i+1],"")){ //caso for nulo
				
				r.sexoBebe = '0';

			} else { //caso nao for nulo

				r.sexoBebe = (*parametros)[2*i+1][0];

			}
		}

		//caso o parametro for estadoMae
		if(0 == strcmp((*parametros)[2*i],"estadoMae")){
			
			if(0 == strcmp((*parametros)[2*i+1],"")){ //caso for nulo
				
				r.estadoMae[0] = '\0';
				r.estadoMae[1] = '$';
				
			} else { //caso nao for nulo

				for(int j = 0; j < 2; j++){
					r.estadoMae[j] = (*parametros)[2*i+1][j];
				}

			}
		}

		//caso o parametro for estadoBebe
		if(0 == strcmp((*parametros)[2*i],"estadoBebe")){
			
			if(0 == strcmp((*parametros)[2*i+1],"")){ //caso for nulo
				
				r.estadoMae[0] = '\0';
				r.estadoMae[1] = '$';
				
			} else { //caso nao for nulo

				for(int j = 0; j < 2; j++){
					r.estadoBebe[j] = (*parametros)[2*i+1][j];
				}

			}
		}

	}

	//chamada da funcao para inserir o registro
	registro_inserir(f, r, 2);

}

/* 
 * Funcao: registro_inserir_fim_aux(FILE *fsource, CABECALHO *c)
 * Parametros:
 * 	FILE *f - arquivo binario para reescrever o registro (Ponteiro deve estar na posicao do registro a ser modificado)
 * 	CABECALHO *c - cabecalho de arquivo de registro que sera atualizado
 * 
 * Retorno: REGISTRO
 * Resumo:
 * 	funcao que insere o novo registro no fim do arquivo de registros
 */
REGISTRO registro_inserir_fim_aux(FILE *fsource, CABECALHO *c){
	//pega a string do registro a ser adicionado a partir da tela
	char entrada[150];
	scanf(" %[^\n\r]%*c", entrada);

	//formata a string para o formato de REGISTRO
	REGISTRO novo = registro_formatar_6(entrada);

	//insercao para registros no fim do arquivo
	if(ftell(fsource) != (c->RRNproxRegistro+1)*128){ //verifica se o ponteiro esta no fim do arquivo, se nao, ele faz um fseek para o fim do arquivo, se sim, ele nao faz nada
		fseek(fsource, (c->RRNproxRegistro+1)*128, SEEK_SET);
	}
	
	registro_inserir(fsource, novo, 0);			//insere o registro no fim do arquivo no modo normal de insercao(0)
	c->numeroRegistrosInseridos++;				//aumenta o numero de registros inseridos
	c->RRNproxRegistro++;						//aumenta o numero do RRNproxRegistro

	return novo;
}

