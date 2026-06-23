#include <stdio.h>
#include <string.h>
#include "arp.h"

int main() {
    inicializar_tabela_arp();

    printf("Testando Insercao\n");
    adicionar_arp("192.168.1.50", "AA:BB:CC:DD:EE:FF");
    adicionar_arp("10.0.0.1", "00:11:22:33:44:55");

    printf("\nTestando Busca (Caso de Sucesso)\n");
    const char* mac_encontrado = buscar_mac("192.168.1.50");
    if (mac_encontrado != NULL) {
        printf("MAC Encontrado: %s\n", mac_encontrado);
    }

    printf("\nTestando Busca (Caso de Falha - Teste C do Relatorio)\n");
    // Isso deve acionar o seu print de erro e retornar NULL
    const char* mac_falha = buscar_mac("192.168.1.99"); 
    if (mac_falha == NULL) {
        printf("O sistema tratou a ausencia do ARP corretamente.\n");
    }

    return 0;
}
