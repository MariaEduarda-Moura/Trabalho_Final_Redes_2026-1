#include <stdio.h>
#include "encaminhamento.h"
#include "arp.h"

/*
 * Motor de forwarding usando a tabela de roteamento REAL do roteador.
 *
 * Etapas:
 *  1. Verifica TTL — se zero, descarta o pacote.
 *  2. Decrementa TTL.
 *  3. Converte IP de destino para inteiro (uint32_t).
 *  4. Itera sobre rt[] do roteador aplicando AND bit a bit:
 *       (ip_dest & mascara) == rede_dest  → rota candidata
 *  5. Aplica Longest Prefix Match (LPM): conta bits da mascara;
 *     a rota com mais bits (prefixo mais longo) vence.
 *  6. Exibe o Next Hop escolhido ou "Destination Unreachable".
 */
void encaminhar_pacote(Pacote *p, Router *roteador_atual) {
    printf("\n--- MOTOR DE ENCAMINHAMENTO (Tabela Real) ---\n");
    printf("[Info] Roteador de origem : %s (%s)\n",
           roteador_atual->nome, roteador_atual->endereco);
    printf("[Info] IP Destino do pacote: %s\n", p->ip_destino);

    /* 1. Verificacao de TTL */
    if (p->ttl <= 0) {
        printf("[Erro] Pacote descartado (TTL = 0) - Time to Live Exceeded.\n");
        return;
    }

    /* 2. Decremento de TTL */
    p->ttl -= 1;
    printf("[Info] TTL decrementado para %d\n", p->ttl);

    /* 3. Conversao do IP destino para inteiro de 32 bits */
    uint32_t ip_dest_int = ip_para_int(p->ip_destino);

    int melhor_prefixo = -1;
    int indice_melhor  = -1;

    /* 4 e 5. AND bit a bit + Longest Prefix Match */
    for (int i = 0; i < roteador_atual->rt_num; i++) {
        uint32_t mascara_int   = ip_para_int(roteador_atual->rt[i].mask);
        uint32_t rede_dest_int = ip_para_int(roteador_atual->rt[i].rede_dest);

        /* AND bit a bit: ip_dest pertence a esta rede? */
        if ((ip_dest_int & mascara_int) == rede_dest_int) {
            int prefixo_atual = contar_prefixo(roteador_atual->rt[i].mask);

            /* LPM: guarda a rota com o prefixo mais longo */
            if (prefixo_atual > melhor_prefixo) {
                melhor_prefixo = prefixo_atual;
                indice_melhor  = i;
            }
        }
    }

    /* 6. Resultado e Consulta ARP */
    if (indice_melhor != -1) {
        printf("[Sucesso] Match encontrado! Rota escolhida via LPM (/%d)\n",
               melhor_prefixo);
        char *next_hop_ip = roteador_atual->rt[indice_melhor].next_hop;
        printf("[Forwarding] Encaminhando para Next Hop: %s\n", next_hop_ip);
        
        printf("[ARP] Consultando MAC para o IP %s...\n", next_hop_ip);
        const char *mac_destino = buscar_mac(next_hop_ip);
        
        if (mac_destino != NULL) {
            printf("[Link Layer] Quadro Ethernet montado com sucesso! MAC Destino: %s\n", mac_destino);
        } else {
            printf("[Erro Link Layer] Falha ao resolver ARP. Pacote descartado.\n");
        }
    } else {
        printf("[Erro] Nenhuma rota encontrada para %s (Destination Unreachable).\n",
               p->ip_destino);
    }
}
