#include <stdio.h>
#include <string.h>
#include "arp.h"

// Variáveis globais
EntradaARP tabela_arp[MAX_ARP_ENTRIES];
int qtd_arp = 0;

// Inicializa a tabela com a memória limpa
void inicializar_tabela_arp() {
    qtd_arp = 0;
    memset(tabela_arp, 0, sizeof(tabela_arp));
}

// Adiciona um mapeamento IP -> MAC na tabela
int adicionar_arp(const char* ip, const char* mac) {
    if (qtd_arp >= MAX_ARP_ENTRIES) {
        printf("[ERRO ARP] Tabela cheia. Não foi possível cadastrar o IP %s\n", ip);
        return 0; // Falha
    }
    
    // Verifica se o IP já existe para só atualizar o MAC
    for (int i = 0; i < qtd_arp; i++) {
        if (strcmp(tabela_arp[i].ip, ip) == 0) {
            strncpy(tabela_arp[i].mac, mac, MAC_LEN - 1);
            return 1;
        }
    }

    // Se IP novo insere no fim do array
    strncpy(tabela_arp[qtd_arp].ip, ip, IP_LEN - 1);
    strncpy(tabela_arp[qtd_arp].mac, mac, MAC_LEN - 1);
    qtd_arp++;
    
    printf("[ARP] Mapeamento adicionado com sucesso: %s -> %s\n", ip, mac);
    return 1;
}

// Busca o MAC por um IP. Se não achar trata o erro
const char* buscar_mac(const char* ip) {
    for (int i = 0; i < qtd_arp; i++) {
        if (strcmp(tabela_arp[i].ip, ip) == 0) {
            return tabela_arp[i].mac; // Retorna o ponteiro para o MAC encontrado
        }
    }

    // Tratamento de Erro (Simulan ARP Request falho e timeout)
    printf("[ERRO ARP] IP %s nao respondeu ao ARP Request.\n", ip);
    printf("[ERRO ARP] Pacote descartado.\n");

    return NULL;
}
