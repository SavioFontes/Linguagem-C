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

#endif