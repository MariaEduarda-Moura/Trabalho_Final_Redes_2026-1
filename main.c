#include <stdio.h>
#include <windows.h>
#undef interface  // windows.h define 'interface' como macro do COM; conflita com struct Router
#include "arp.h"
#include "table_structs.h"
#include "pacotes.h"

int main() {
    SetConsoleOutputCP(65001);
    inicializar_tabela_arp();
    char opcao;
    Pacote pacote_atual;
    int pacote_criado = 0;

    char ip_orig[16], ip_dest[16], dados[100];
    int ttl;
    Network topTopologia;
    topTopologia.router_count = 0;

    while(opcao != '9'){
      printf("Qual menu você quer acessar?\n");
      printf("1 - Topologia de rede e roteadores\n");
      printf("2 - Criação e encaminhamento de pacotes\n");
      printf("9 - Sair\n");
      scanf("%c",&opcao);
      switch(opcao){
        case '1':
          table_menu(&topTopologia);
          break;        
        case '2':
          pacote_menu();
          break;
        case '9':
          break;
        default: printf("Selecione uma opção válida\n");
        break; 
      }
    }
    printf("Até mais!\n");
    return 0;

    /*
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
    */
          
}
