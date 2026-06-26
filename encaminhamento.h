#ifndef ENCAMINHAMENTO_H
#define ENCAMINHAMENTO_H

#include "pacotes.h"
#include "table_structs.h"

/*
 * Motor de forwarding principal.
 * Usa a tabela de roteamento real do roteador (calculada via Dijkstra).
 * Aplica: verificacao de TTL, AND bit a bit, Longest Prefix Match (LPM).
 */
void encaminhar_pacote(Pacote *p, Router *roteador_atual);

#endif
