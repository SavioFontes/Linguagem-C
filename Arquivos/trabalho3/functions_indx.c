#include"functions_indx.h"


/*
 * Funcao: abrir_arquivo_indx(char *source, int modo, FILE **abrir, int* funciona)
 * Parametros:
 * 	char *source - nome do arquivo source para ser aberto
 * 	int modo - modo de abertura do arquivo: 0 - modo rb | 1 - modo rb+
 * 	FILE **abrir - ponteiro para o arquivo a ser aberto
 * 	int *funciona - flag para verificacao do sucesso de abertura do arquivo
 * 
 * Retorno: CABECALHO_INDX - CABECALHO_INDX do arquivo aberto
 * Resumo:
 * 	Abre o arquivo com o nome "source" e pega seu CABECALHO_INDX.
 * 	Verifica o status do arquivo, caso o modo seje rb+ e wb+, o status ja eh atualizado para 0.
 */
CABECALHO_INDX abrir_arquivo_indx(char *source, int modo, FILE **abrir, int* funciona){

	if(modo == 0) *abrir = fopen(source, "rb"); //abre o arquivo modo rb
	if(modo == 1) *abrir = fopen(source, "rb+");//abre o arquivo modo rb+
	if(modo == 2) *abrir = fopen(source, "wb+");//abre o arquivo modo wb+ 
	
	CABECALHO_INDX c; //CABECALHO_INDX do arquivo

	if(*abrir == NULL){ //caso arquivo inexistente
		
		*funciona = 0; //flag que verifica se o arquivo se esta apto para operacoes
					
		printf("Falha no processamento do arquivo.\n");
					
	} else{ //caso arquivo existente

		if(modo == 2){
			c.status = '0';			//
			c.noRaiz = -1;			//
			c.nroNiveis = 0;		//recebendo parametros conforme a iniciacao do arquivo
			c.proxRRN = 0;			//
			c.nroChaves = 0;		//

			cabecalho_indx_atualizar(*abrir, c);		//insere CABECALHO_INDX do arquivo binario
			*funciona = 1;
			return c;
		}
		
		c = cabecalho_indx_leitura(*abrir); //le o CABECALHO_INDX do arquivo
	
		if(c.status == '0'){ //caso o arquivo esteja com status de invalido

			printf("Falha no processamento do arquivo.\n");
			fclose(*abrir); //fecha o arquivo
			*abrir = NULL;
			*funciona = 0; //flag que verifica se o arquivo se esta apto para operacoes
		
		} else{ //caso arquivo esteje valido
		
			*funciona = 1; //flag que verifica se o arquivo se esta apto para operacoes
		
			if(modo == 1){ //caso modo seje rb+, já deixa o status sendo '0'
				
				c.status = '0';
				cabecalho_indx_atualizar(*abrir,c); //atualiza o CABECALHO_INDX
			
			}
		}
	}

	return c; //retorna o CABECALHO_INDX da funcao
}

/*
 * Funcao: cabecalho_indx_atualizar(FILE *p, CABECALHO c)
 * Parametros:
 * 	FILE *p - arquivo para atualizar cabecaclho;
 * 	CABECALHO c - cabecalho a ser atualizado no arquivo
 * Retorno: void
 * Resumo:
 * 	Atualiza o cabecalho, passando a struct "CABECALHO" como parametro a ser colocada na posicao do cabecalho
 */
void cabecalho_indx_atualizar(FILE *p, CABECALHO_INDX c){
	
	//atualiza o cabecalho do binario 

	fseek(p, 0, SEEK_SET);									//fseek para o inicio do arquivo
	fwrite(&c.status, sizeof(char), 1, p);					//carrega o status
	fwrite(&c.noRaiz, sizeof(int), 1, p);					//carrega o RRNproxRegistro
	fwrite(&c.nroNiveis, sizeof(int), 1, p);				//carrega o numeroRegistrosInseridos
	fwrite(&c.proxRRN, sizeof(int), 1, p);					//carrega o numeroRegistrosRemovidos
	fwrite(&c.nroChaves, sizeof(int), 1, p);				//carrega o numeroRegistrosAtualizados

	//criacao do lixo para o cabecalho ter o tamanho de um registro
	for(int i = 0; i < 55; i++){
		c.lixo[i] = '$';
	}
	fwrite(c.lixo, sizeof(char), 55, p);					//escreve o lixo
}

/*
 * Funcao: cabecalho_indx_leitura(FILE *s)
 * Parametros:
 * 	FILE *s - arquivo fonte para ler o cabecalho, o ponteiro do arquivo deve estar no byte offset 0;
 * Retorno: CABECALHO
 * Resumo:
 * 	Le o cabecalho do arquivo_inx e retorna ele na struct em formato de cabecalho_indx
 */
CABECALHO_INDX cabecalho_indx_leitura(FILE *s){

	CABECALHO_INDX c; //cabecalho a ser retornado

	//leitura das partes do cabecalho
	fread(&c.status,sizeof(char), 1, s);
	fread(&c.noRaiz,sizeof(int), 1, s);
	fread(&c.nroNiveis,sizeof(int), 1, s);
	fread(&c.proxRRN,sizeof(int), 1, s);
	fread(&c.nroChaves,sizeof(int), 1, s);
	fread(c.lixo,sizeof(char), 55, s);

	return c;
}

/*
 * Funcao: indx_inserir(FILE *f, CABECALHO_INDX *c, int RRN_indx_atual, int key, int RRN_key, int *promo_key, int *RRN_promo_key, int *promo_r_child)
 * Parametros:
 * 	FILE *f - arquivo de indx
 *  int key - idNascimento a ser inserido
 *  int RRN_key	-	RRN do registro no arquivo de registros
 * 	CABECALHO_INDX *c - cabecalho que sera atualizado
 * 	PAGINA_INDX *p_atual - Pagina atual
 * 	int *promo_key - idNascimento a ser promovido(caso haja promocao)
 * 	int *RRN_promo_key - RRN a key promovida
 * 	int *promo_r_child - filho direito da key promovida
 * 
 * Retorno: int retorna 0 = promotion/ retorna 1 = not promotion / retorna 2 = erro
 * Resumo:
 * 	insere a key no arquivo de indx seguindo o modelo de arvoreB
 * 
 */
int indx_inserir(FILE *f, CABECALHO_INDX *c, int RRN_indx_atual, int key, int RRN_key, int *promo_key, int *RRN_promo_key, int *promo_r_child){
	
	//promotion: quando p.n == 5
	if(RRN_indx_atual == -1){			//caso a pagina atual nao exista
		*promo_key = key;				//retorna no ponteiro a propria key
		*promo_r_child = -1;			//nao existe um filho a direita
		*RRN_promo_key = RRN_key;		//retorna o RRN da propria key
		return 0;
	}else{
		int valor_retorno;								//valor a ser retornado como os diferentes tipos

		fseek(f,(RRN_indx_atual+1)*72,SEEK_SET);		//move o ponteiro do arquivo para a pagina desejada
		PAGINA_INDX  p = indx_leitura(f);				//le a pagina

		for(int i=0;i<p.n;i++){							//loop de keys
			if(key == p.idNascimento[i])return 2;		//caso ja exista a key retorna ERRO
			if(key < p.idNascimento[i]){				//caso seja menor que o id nascimento continua caminhando a arvore
				valor_retorno = indx_inserir(f,c,p.RRN_indx[i],key,RRN_key,promo_key,RRN_promo_key,promo_r_child);
				break;
			}
			
		}

		if(key > p.idNascimento[p.n-1]){				//caso seja maior que todos os idNascimentos da pagina
			valor_retorno = indx_inserir(f,c,p.RRN_indx[p.n],key,RRN_key,promo_key,RRN_promo_key,promo_r_child);
		}

		if(valor_retorno == 2 || valor_retorno == 1) {					//caso não houve necessidade promotion ou houve erro
			return valor_retorno;
		}else{
			if(p.n < 5){												//caso houve promotion e a key cabe na pagina
				
				for(int i=0;i<p.n;i++){
					if(*promo_key < p.idNascimento[i]){							//se a key inserida é menor que a key daquela posicao 
						for(int j=p.n;j>i;j--){									//transfere as key para as posicoes posteriores
							p.idNascimento[j] = p.idNascimento[j-1];
							p.RRNidNascimento[j] = p.RRNidNascimento[j-1];
							p.RRN_indx[j+1] = p.RRN_indx[j];					//transfere as filhas a direita para as posicoes posteriores
						}
						p.idNascimento[i] = *promo_key;					
						p.RRNidNascimento[i] = *RRN_promo_key;					//posiciona a key dentro da pagina
						p.RRN_indx[i+1] = *promo_r_child;
						break;
					}
				}

				if(*promo_key>p.idNascimento[p.n-1]){							//caso a key inserida seja maior que todas as keys da pagina
					p.idNascimento[p.n] = *promo_key;					
					p.RRNidNascimento[p.n] = *RRN_promo_key;
					p.RRN_indx[p.n+1] = *promo_r_child;
				}
				
				p.n++;													//atualiza o numero de keys da pagina
				c->nroChaves++;											//atualiza o numero de key do arquivo

				fseek(f,(RRN_indx_atual+1)*72,SEEK_SET);				//avanca o ponteiro para o byte-offset da pagina a atualizada
				indx_escrita(f,p);										//escreve a pagina
				
				return 1;	//retorna NO PROMOTION
			}else{														//caso houve promotion e a key nao cabe na pagina
				
				PAGINA_INDX p_nova = split(c,&p,promo_key,RRN_promo_key,promo_r_child);		//chama a funcao split caso na exista espaco na pagina
				
				fseek(f,(RRN_indx_atual+1)*72,SEEK_SET);		//move o ponteiro para a posicao da pagina("esquerda") e a escreve
				indx_escrita(f,p);
				
				fseek(f,(*promo_r_child+1)*72,SEEK_SET);		//move o ponteiro para a posicao da pagina("direita") e a escreve
				indx_escrita(f,p_nova);
				
				return 0;	//retorna PROMOTION
			}
		}
	
	}
	
	
}

/*
 * Funcao: split(CABECALHO_INDX *c, PAGINA_INDX *p_atual, int *promo_key, int *RRN_promo_key, int *promo_r_child)
 * Parametros:
 * 	CABECALHO_INDX *c - cabecalho que sera atualizado
 * 	PAGINA_INDX *p_atual - Pagina atual
 * 	int *promo_key - idNascimento a ser promovido(caso haja promocao)
 * 	int *RRN_promo_key - RRN a key promovida
 * 	int *promo_r_child - filho direito da key promovida
 * 
 * Retorno: PAGINA_INDX
 * Resumo:
 * 	Da split na pagina atual criando uma nova e promovendo a key do meio
 * 
 */
PAGINA_INDX split(CABECALHO_INDX *c, PAGINA_INDX *p_atual, int *promo_key, int *RRN_promo_key, int *promo_r_child){
	
	PAGINA_INDX p_novo;						//cria nova pagina
	WORKNG_PAGE_INDX w;						//cria a pagina com tamanho maior para fazer o split
	w.n = p_atual->n;
	
	for(int i=0;i<w.n;i++){												//atualiza a pagina a maior com as informacoes da pagina que esta full
		w.idNascimento[i] = p_atual->idNascimento[i];
		w.RRNidNascimento[i] = p_atual->RRNidNascimento[i];
		w.RRN_indx[i] = p_atual->RRN_indx[i];
	}
	w.RRN_indx[w.n] = p_atual->RRN_indx[p_atual->n];

	
	for(int i=0;i<w.n;i++){												//insere a key na pagina maior(WORKING_PAGE_INDX)
		if(*promo_key < w.idNascimento[i]){
			for(int j=w.n;j>i;j--){
				w.idNascimento[j] = w.idNascimento[j-1];
				w.RRNidNascimento[j] = w.RRNidNascimento[j-1];
				w.RRN_indx[j+1] = w.RRN_indx[j];
			}
			w.idNascimento[i] = *promo_key;
			w.RRNidNascimento[i] = *RRN_promo_key;
			w.RRN_indx[i+1] = *promo_r_child;
			break;
		}
	}
	

	if(*promo_key>w.idNascimento[w.n-1]){										//caso a key inserida seja maior que todas as keys anteriores
		w.idNascimento[w.n] = *promo_key;					
		w.RRNidNascimento[w.n] = *RRN_promo_key;
		w.RRN_indx[w.n+1] = *promo_r_child;
	}
	
	w.n++;								//aumenta o numero de keys na pagina maior

	*promo_key = w.idNascimento[3];				//atualiza promo_key como o meio da pagina maior
	*promo_r_child = c->proxRRN;				//pega a a ultima posicao do arquivo
	c->proxRRN++;								//incrementa no ultimo RRN do arquivo indx	
	*RRN_promo_key = w.RRNidNascimento[3];		//atualiza com o RRN do registro

	p_atual->n = 3;								//como houve o split a pagina recebe a primeira metade da pagina maior
	p_novo.n = 2;								//como a key do meio foi promovida a pagina nova recebe as duas ultimas keys da pagina maior
	p_novo.nivel = p_atual->nivel;				//pagina nova recebe o mesmo nivel da pagina atual

	for(int i=0;i<6;i++){											//loop de split das keys(em pagina da esquerda e da direita)
		if(i<3){													//filha da esquerda
			p_atual->idNascimento[i] = w.idNascimento[i];
			p_atual->RRNidNascimento[i] = w.RRNidNascimento[i];
			p_atual->RRN_indx[i] = w.RRN_indx[i];
		}
		if(i>3){													//filha da direita
			p_novo.idNascimento[i-4] = w.idNascimento[i];
			p_novo.RRNidNascimento[i-4] = w.RRNidNascimento[i];
			p_novo.RRN_indx[i-4] = w.RRN_indx[i];
		}
	}

	p_novo.RRN_indx[p_novo.n] = w.RRN_indx[6];			//atualizacao do filho direito das ultimas keys das respectivas paginas
	p_atual->RRN_indx[p_atual->n] = w.RRN_indx[3];
	
	
	return p_novo;			//retorna a pagina da direita
}

/*
 * Funcao: indx_busca(int RRN_indx, int idNascimento, int *RRN_reg, FILE *f, int *nro_paginas)
 * Parametros:
 * 	int RRN_indx - RRN da pagina atual
 * 	int idNascimento - key buscada
 * 	int *RRN_reg - ponteiro que salva o RRN do registro
 * 	FILE *f - arquivo de indx
 * 	int *nro_paginas - ponteiro para contagem de paginas lidas
 * 
 * Retorno: void
 * Resumo:
 * 	Faz uma busca recursiva no arquivo indx de um idNascimento desejado
 * 
 */
void indx_busca(int RRN_indx, int idNascimento, int *RRN_reg, FILE *f, int *nro_paginas){

	if(RRN_indx == -1){						//caso nao exista a pagina(chegou ao fim sem encontrar)		
		*RRN_reg = -1;						//RRN_reg como não existe
		return; //NOT FOUND
	}else{
		*nro_paginas = *nro_paginas + 1;							//incrementa o numero de paginas visitadas
		fseek(f,(RRN_indx+1)*72,SEEK_SET);							//move o ponteiro para a pagina desejada	
		PAGINA_INDX p = indx_leitura(f);							//leitura da pagina
		for(int i=0;i<p.n;i++){										//loop das keys
			if(idNascimento == p.idNascimento[i]){					//caso encontre a key retorna
				*RRN_reg = p.RRNidNascimento[i];					//poe o valor do RRN do arquivo de registro no ponteiro
				return; //FOUND
			} 
			if(idNascimento < p.idNascimento[i]){					//caso seja menor continua a busca pela arvore
				return indx_busca(p.RRN_indx[i],idNascimento,RRN_reg,f,nro_paginas);
			}
			
		}
																			
		return indx_busca(p.RRN_indx[p.n],idNascimento,RRN_reg,f,nro_paginas);	//caso seja maior que todas as keys da pagina

	}
 
}

/*
 * Funcao: indx_leitura(FILE *p)
 * Parametros:
 * 	FILE *p - arquivo product a ser escrito;
 *
 * Retorno: PAGINA_INDX
 * Resumo:
 * 	Leitura da Pagina_indx p  do arquivo binario de indices.
 * 
 */
PAGINA_INDX indx_leitura(FILE *f){
	
	PAGINA_INDX p;
	fread(&p.nivel,sizeof(int),1,f);
	fread(&p.n,sizeof(int),1,f);

	//leitura das keys e seus RRNs
	for(int i=0;i<5;i++){							
		fread(&p.idNascimento[i],sizeof(int),1,f);
		fread(&p.RRNidNascimento[i],sizeof(int),1,f);			
	}

	//leitura das paginas filhas
	for(int i=0;i<6;i++){
		fread(&p.RRN_indx[i],sizeof(int),1,f);
	}

	return p;
}

/*
 * Funcao: indx_escrita(FILE *f, PAGINA_INDX p)
 * Parametros:
 * 	FILE *p - arquivo product a ser escrito
 * 	PAGINA_INDX p - pagina a ser escrita 	
 *
 * Retorno: void
 * Resumo:
 * 	Escreve a Pagina_indx p  ao arquivo binario de indices.
 * 
 */
void indx_escrita(FILE *f, PAGINA_INDX p){
	
	int nil = -1;						//variavel com valor para filhas nulos
	
	fwrite(&p.nivel,sizeof(int),1,f);
	fwrite(&p.n,sizeof(int),1,f);

	//escreve  as keys, primeiro a key e depois seu RRN no arquivo de registro
	for(int i=0;i<p.n;i++){
		fwrite(&p.idNascimento[i],sizeof(int),1,f);
		fwrite(&p.RRNidNascimento[i],sizeof(int),1,f);			
	}

	//loop que escreve os espacos vazios caso haja
	for(int i=p.n;i<5;i++){
		fwrite(&nil,sizeof(int),1,f);
		fwrite(&nil,sizeof(int),1,f);
	}

	//escreve o RRN do arquivo de indx das paginas filhas
	for(int i=0;i<=p.n;i++){
		fwrite(&p.RRN_indx[i],sizeof(int),1,f);
	}

	//loop que seta os filhos dos espacos vazios como -1;
	for(int i=p.n+1;i<6;i++){
		fwrite(&nil,sizeof(int),1,f);
	}

}

/*
 * Funcao: indx_nova_raiz(FILE *f, CABECALHO_INDX *c_indx,int promo_key, int RRN_promo_key, int promo_r_child )
 * Parametros:
 * 	FILE *p - arquivo product a ser escrito;
 * 	CABECALHO_INDX *c_indx - cabecalho de indx a ser atualizado
 * 	int promo_key - idNascimento promovido
 * 	int RRN_promo_key - RRN do registro promovido
 * 	int promo_r_child - RRN de indx do filho direito
 *
 * Retorno: void
 * Resumo:
 * 	
 * Cria uma nova pagina indx insere a key promovida e atualiza o cabecalho no caso do no raiz sofrer um split
 * 
 */
void indx_nova_raiz(FILE *f, CABECALHO_INDX *c_indx,int promo_key, int RRN_promo_key, int promo_r_child ){
	
	PAGINA_INDX p;
	p.idNascimento[0] = promo_key;						//insere a key promovida
	p.RRNidNascimento[0] = RRN_promo_key;				//insere o RRN de registro da key promovida
	p.RRN_indx[0] = c_indx->noRaiz;						//insere como filho a esquerda a antiga raiz da arvore
	p.RRN_indx[1] = promo_r_child;						//insere como a direita o filho-a-direita promovido	
	p.n = 1;											//atualiza o numero de keys na pagina
	p.nivel = c_indx->nroNiveis + 1;					//atualiza o nivel da pagina;

	c_indx->noRaiz = c_indx->proxRRN;						//atualiza o RRN do no raiz
	c_indx->nroNiveis++;									//atualiza o numero de niveis da arvore
	c_indx->proxRRN++;										//atualiza o ultimo RRN do arquivo
	c_indx->nroChaves++;									//atualiza o numero de chaves	
	
	
	fseek(f,(c_indx->noRaiz+1)*72,SEEK_SET);				//avanca o ponteiro para o ultimo byte-offset
	indx_escrita(f,p);										//escreve a pagina no arquivo
}
