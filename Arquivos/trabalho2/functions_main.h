#ifndef FUNCTIONS_MAIN_H
#define FUNCTIONS_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions_aux.h"
#include "binarioNaTela.h"
#include "structs.h"


int registro_transferir(char *source, char *product);

int registro_imprimir(char *source);

int registro_busca(char *source);

int registro_rrn_imprimir(char *source);

int registro_remocao_logica(char *source);

int registro_inserir_fim(char *source);

int registro_atualizar(char *source);

#endif