#ifndef FUNCTIONS_INDX_H
#define FUNCTIONS_INDX_H

#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

CABECALHO_INDX abrir_arquivo_indx(char *source, int modo, FILE **abrir, int* funciona);

void cabecalho_indx_atualizar(FILE *p, CABECALHO_INDX c);

CABECALHO_INDX cabecalho_indx_leitura(FILE *s);

int indx_inserir(FILE *f, CABECALHO_INDX *c, int RRN_indx_atual, int key, int RRN_key, int *promo_key, int *RRN_promo_key, int *promo_r_child);

void indx_busca(int RRN_indx, int idNascimento, int *RRN_reg, FILE *f, int *nro_paginas);

PAGINA_INDX indx_leitura(FILE *f);

void indx_escrita(FILE *f, PAGINA_INDX p);

PAGINA_INDX split(CABECALHO_INDX *c, PAGINA_INDX *p_atual, int *promo_key, int *RRN_promo_key, int *promo_r_child);

void indx_nova_raiz(FILE *f, CABECALHO_INDX *c_indx,int promo_key, int RRN_promo_key, int promo_r_child );

#endif