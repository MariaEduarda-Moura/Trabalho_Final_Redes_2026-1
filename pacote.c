#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char ip_origem[16];  // Guarda o IP de quem envia. Tem tamanho 16 para caber o formato "XXX.XXX.XXX.XXX" + o fim do texto (\0)
    char ip_destino[16]; // Guarda o IP de quem vai receber.
    int ttl;             // Time to Live (Tempo de Vida) - número inteiro que diminui a cada salto
    char dados[100];     // O conteúdo/mensagem que o pacote carrega (limite de 100 caracteres)
} Pacote;

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
int main() {
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
                printf("\nEspaco reservado para insercao de rotas.\n");
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
                    printf("\nO motor de encaminhamento processara este pacote em breve...\n");
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