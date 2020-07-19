#ifndef FUNCTIONS_AUX_H
#define FUNCTIONC_AUX_H

#include "functions_main.h"
#include "structs.h"

CABECALHO abrir_arquivo(char *source, int modo, FILE **abrir, int* funciona);

void descarte_primeira_linha(FILE *c);

REGISTRO registro_formatar(char *s);

void registro_inserir(FILE *p, REGISTRO novo, int modo);

void cabecalho_atualizar(FILE *p, CABECALHO c);

CABECALHO cabecalho_leitura(FILE *s);

REGISTRO registro_retorna(FILE *s, int *excluido);

void imprimir_formatado(REGISTRO r);

int registro_verifica_busca(REGISTRO r, int m, char parametros[1][2*m][50]);

REGISTRO registro_formatar_6(char *s);

void registro_reescrever(FILE *f, REGISTRO r, int m, char parametros[1][2*m][50]);

REGISTRO registro_inserir_fim_aux(FILE *fsource, CABECALHO *c);

#endif