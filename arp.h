#define ARP_H

#define MAX_ARP_ENTRIES 100
#define IP_LEN 16 //Modelo IPv4
#define MAC_LEN 18

typedef struct {
    char ip[IP_LEN];    // 192.168.1.1
    char mac[MAC_LEN];  // 00:1A:3F:F1:4C:C6
} EntradaARP;

// Declaração da Tabela ARP e seu contador
extern EntradaARP tabela_arp[MAX_ARP_ENTRIES];
extern int qtd_arp;

// Protótipos das funções
void inicializar_tabela_arp();
int adicionar_arp(const char* ip, const char* mac);
const char* buscar_mac(const char* ip);
