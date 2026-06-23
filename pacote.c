#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "pacotes.h"

// Esta função recebe os dados digitados e devolve um "Pacote" prontinho.
Pacote criar_pacote(char *origem, char *destino, int ttl, char *conteudo) {
    Pacote p; // Cria uma variável temporária do tipo Pacote chamada 'p'

    strcpy(p.ip_origem, origem);
    strcpy(p.ip_destino, destino);

    p.ttl = ttl;
    
    strcpy(p.dados, conteudo);

    return p; 
}

void imprimir_pacote(Pacote p) {
    printf("\n====================================\n");
    printf("         DADOS DO PACOTE            \n");
    printf("====================================\n");
    printf(" IP Origem  : %s\n", p.ip_origem);
    printf(" IP Destino : %s\n", p.ip_destino);
    printf(" TTL Atual  : %d\n", p.ttl);
    printf(" Conteudo   : %s\n", p.dados);
    printf("====================================\n");
}

// ====================================================================
// INÍCIO: ESTRUTURAS AUXILIARES PARA FACILITAR A INTEGRAÇÃO (FRAN)
// ====================================================================

RotaTeste tabela_rotas_teste[100];
int qtd_rotas_teste = 0;

// Função auxiliar: Converte IP em string para inteiro de 32 bits
uint32_t ip_para_int(const char *ip_str) {
    uint32_t p[4];
    if (sscanf(ip_str, "%u.%u.%u.%u", &p[0], &p[1], &p[2], &p[3]) != 4) return 0;
    return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

// Função auxiliar: Conta o número de bits 1 na máscara (Longest Prefix)
int contar_prefixo(const char *mask_str) {
    uint32_t mask = ip_para_int(mask_str);
    int count = 0;
    while (mask) {
        count += (mask & 1);
        mask >>= 1;
    }
    return count;
}

// ====================================================================
// FIM: ESTRUTURAS AUXILIARES
// ====================================================================

// --- MOTOR DE ROTEAMENTO (PASSO 4) ---
void encaminhar_pacote(Pacote *p) {
    printf("\n--- INICIANDO MOTOR DE ROTEAMENTO ---\n");
    if (p->ttl <= 0) {
        printf("[Erro] Pacote descartado (TTL = 0) - Time to Live Exceeded.\n");
        return;
    }

    p->ttl -= 1; // Decrementa TTL
    printf("[Info] TTL decrementado para %d\n", p->ttl);

    uint32_t ip_dest_int = ip_para_int(p->ip_destino);
    int melhor_prefixo = -1;
    int indice_melhor_rota = -1;

    // Busca na tabela de rotas com Longest Prefix Match (LPM)
    for (int i = 0; i < qtd_rotas_teste; i++) {
        uint32_t mascara_int = ip_para_int(tabela_rotas_teste[i].mask);
        uint32_t rede_dest_int = ip_para_int(tabela_rotas_teste[i].rede_dest);

        // Operação AND bit a bit para achar a rede
        if ((ip_dest_int & mascara_int) == rede_dest_int) {
            int prefixo_atual = contar_prefixo(tabela_rotas_teste[i].mask);
            if (prefixo_atual > melhor_prefixo) {
                melhor_prefixo = prefixo_atual;
                indice_melhor_rota = i;
            }
        }
    }

    if (indice_melhor_rota != -1) {
        printf("[Sucesso] Match encontrado! Rota escolhida via LPM (/%d)\n", melhor_prefixo);
        printf("[Forwarding] Encaminhando pacote para Next Hop: %s\n", tabela_rotas_teste[indice_melhor_rota].next_hop);
    } else {
        printf("[Erro] Nenhuma rota encontrada para o destino %s (Destination Unreachable).\n", p->ip_destino);
    }
}
// -----------------------------------------------------------------

int pacote_menu() {
    int opcao;
    Pacote pacote_atual;
    int pacote_criado = 0; 

    char ip_orig[16], ip_dest[16], dados[100];
    int ttl;

    do {
        printf("\n====================================\n");
        printf("       MINI-ROTEADOR SIMULADOR      \n");
        printf("====================================\n");
        printf(" 1. Cadastrar Rota\n");
        printf(" 2. Cadastrar ARP\n");
        printf(" 3. Criar Novo Pacote\n");
        printf(" 4. Enviar/Encaminhar Pacote\n");
        printf(" 5. Visualizar Pacote Atual\n");
        printf(" 0. Sair\n");
        printf("------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                printf("\n--- CADASTRAR ROTA ---\n");
                printf("Rede de Destino (Ex: 192.168.1.0): ");
                fgets(tabela_rotas_teste[qtd_rotas_teste].rede_dest, 16, stdin);
                tabela_rotas_teste[qtd_rotas_teste].rede_dest[strcspn(tabela_rotas_teste[qtd_rotas_teste].rede_dest, "\n")] = 0;

                printf("Mascara (Ex: 255.255.255.0): ");
                fgets(tabela_rotas_teste[qtd_rotas_teste].mask, 16, stdin);
                tabela_rotas_teste[qtd_rotas_teste].mask[strcspn(tabela_rotas_teste[qtd_rotas_teste].mask, "\n")] = 0;

                printf("Next Hop (Proximo Roteador): ");
                fgets(tabela_rotas_teste[qtd_rotas_teste].next_hop, 16, stdin);
                tabela_rotas_teste[qtd_rotas_teste].next_hop[strcspn(tabela_rotas_teste[qtd_rotas_teste].next_hop, "\n")] = 0;

                qtd_rotas_teste++;
                printf("[Sucesso] Rota cadastrada localmente para testes do motor!\n");
                break;
                
            case 2:
                printf("\nEspaco reservado para mapeamento ARP.\n");
                break;
                
            case 3:
                printf("\n--- CRIANDO NOVO PACOTE ---\n");
                printf("IP de Origem (Ex: 192.168.1.1): ");
                fgets(ip_orig, 16, stdin); 
                ip_orig[strcspn(ip_orig, "\n")] = 0;
                
                printf("IP de Destino (Ex: 10.0.0.5): ");
                fgets(ip_dest, 16, stdin); 
                ip_dest[strcspn(ip_dest, "\n")] = 0;
                
                printf("Defina o TTL (Tempo de Vida - Ex: 64): ");
                scanf("%d", &ttl);
                getchar();
                
                printf("Conteudo/Dados do Pacote: ");
                fgets(dados, 100, stdin); 
                dados[strcspn(dados, "\n")] = 0;

                // Chama a sua funcao do Passo 1 salvando o retorno na struct global
                pacote_atual = criar_pacote(ip_orig, ip_dest, ttl, dados);
                pacote_criado = 1;
                printf("\n[Sucesso] Pacote gerado e armazenado na memoria!\n");
                break;
                
            case 4:
                if (pacote_criado == 0) {
                    printf("\n[Erro] Voce precisa criar um pacote primeiro (Opcao 3).\n");
                } else {
                    encaminhar_pacote(&pacote_atual);
                }
                break;
                
            case 5:
                if (pacote_criado == 0) {
                    printf("\n[Aviso] Nenhum pacote foi criado ate o momento.\n");
                } else {
                    imprimir_pacote(pacote_atual);
                }
                break;
                
            case 0:
                printf("\nSaindo do simulador. Ate logo!\n");
                break;
                
            default:
                printf("\n[Erro] Opcao invalida. Tente novamente.\n");
        }
    } while(opcao != 0);

    return 0;
}
