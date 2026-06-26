#ifndef PACOTES_H
#define PACOTES_H

#include <stdint.h>
#include "table_structs.h"

typedef struct {
    char ip_origem[16];  // Guarda o IP de quem envia. Tem tamanho 16 para caber o formato "XXX.XXX.XXX.XXX" + o fim do texto (\0)
    char ip_destino[16]; // Guarda o IP de quem vai receber.
    int ttl;             // Time to Live (Tempo de Vida) - número inteiro que diminui a cada salto
    char dados[100];     // O conteúdo/mensagem que o pacote carrega (limite de 100 caracteres)
} Pacote;

typedef struct {
    char rede_dest[16];
    char mask[16];
    char next_hop[16];
} RotaTeste;

Pacote criar_pacote(char *origem, char *destino, int ttl, char *conteudo);
void imprimir_pacote(Pacote p);
uint32_t ip_para_int(const char *ip_str);
int contar_prefixo(const char *mask_str);
void encaminhar_pacote_teste(Pacote *p);
int pacote_menu(Network *net);

#endif
