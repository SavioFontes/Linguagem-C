/*
 * Biblioteca com as structs que são utilizadas no programa
 */

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct{
	char status;
	int RRNproxRegistro;
	int numeroRegistrosInseridos;
	int numeroRegistrosRemovidos;
	int numeroRegistrosAtualizados;
	char lixo[111];	//lixo correspondente aos 111 bytes nao utilizados
} CABECALHO;

typedef struct{
	int tamCidadeMae;       //tamanho do campo cidadeMae (tamanho fixo de 4 bytes)
	int tamCidadeBebe;  		//tamanho do campo cidadeBebe (tamanho fixo de 4 bytes)
	char cidadeMae[50];  //cidadeMae (tamanho variável)
	char cidadeBebe[50];  //cidadeBebe (tamanho variável)
	int idNascimento;  		//idNascimento (tamanho fixo de 4 bytes)
	int idadeBebe;  		//idadeMae (tamanho fixo de 4 bytes)
	char dataNascimento[10];  //dataNascimento (tamanho fixo de 10 bytes)
	char sexoBebe;  		//sexoBebe (tamanho fixo de 1 byte)
	char estadoMae[2];  	//estadoMae (tamanho fixo de 2 bytes)
	char estadoBebe[2]; 	//estadoBebe (tamanho fixo de 2 bytes)
} REGISTRO;

typedef struct{
	char status;		//consistencia do arquivo
	int noRaiz;			//inicio da arvore
	int nroNiveis;		//numero de niveis
	int proxRRN;		//proxima pagina a ser escrita
	int nroChaves;		//numero de chaves no arquivo de index
	char lixo[55];		//lixo correspondente aos 111 bytes nao utilizados
} CABECALHO_INDX;

typedef struct{
	int nivel;						//nivel do no
	int n;							//numero de chaves presentes no no
	int idNascimento[5];			//array com chaves do no(idNascimento)
	int RRNidNascimento[5];			//array com RRNs das chaves do no
	int RRN_indx[6];				//array com RRNs para as sub-arvores do no
}PAGINA_INDX;

typedef struct{						//struct usada no split da pagina
	int n;							//numero de chaves presentes no no
	int idNascimento[6];			//array com chaves do no(idNascimento)
	int RRNidNascimento[6];			//array com RRNs das chaves do no
	int RRN_indx[7];				//array com RRNs para as sub-arvores do no
}WORKNG_PAGE_INDX;

#endif