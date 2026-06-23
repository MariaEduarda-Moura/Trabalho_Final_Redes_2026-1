#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table_structs.h"

/* Codigo da tabela de roteamento. As funcoes associadas como criar, deletar
 * e consultar estão aqui */

/* Modelo esperado da struct tabela:
 * string rede_dest;
 * string mask;
 * string next_hop;
 * float cost;
*/

/* Qual a melhor forma de guardar uma tabela? Ponteiros.
 * Brincadeira, acho q vou usar arquivo... parece sabio. Cada nó tem uma tabela
 * dessa
 * Pra criar rotas vou usar djikstra :)  Dai meio q tem que iterar sobre uma
 * lista de nós.
 */ 

// Cada router é um nó... os apontamentos dele são feitos olhando para
// a estrutura de tabela dentro dele;
//
// Pergunta: Como eu cálculo/defino o custo de um roteador ao outro?
// Eu posso ter uma função que, apartir de um roteador, testa o tempo que leva
// pras respostas viajarem. Isso é o meu peso.
// Com essas informações, eu construo uma matriz/Grafos com peso
//
// Como eu sei quais roteadores estão na rede?
//    Eu meio que já tenho um grafo, porque as conexões entre roteadores sou eu
//    que tem que criar KKKKKKKKKKKKKKKKKKKKK
//    .... Mano... será que da pra deixar isso interativo? tipo, clica
//    e arrasta? seria tipo, o céu; 

// Eu poderia ter um vetor estático tb, e uma variavel pra guardar
// o número de conexões que um dado nó tem...

int better_strncpy(char* dest, char* src, int len){
  strncpy(dest,src, len-1);
  dest[len-1] = '\0';
  return 1;
} 

Router* create_router(char * endereco, char * nome , char * interface ){
  Router* r1 = malloc(sizeof(Router));
  if (r1 == NULL) return NULL;
  strncpy(r1->endereco,endereco, sizeof(r1->endereco)-1); 
  r1->nome = malloc(strlen(nome)+1);
  if (r1->nome == NULL) {
    free(r1);
    return NULL;
  }
  strcpy(r1->nome, nome);
  strncpy(r1->interface,interface,sizeof(r1->interface)-1);

  r1->endereco[sizeof(r1->endereco)-1] = '\0';
  r1->interface[sizeof(r1->interface)-1] = '\0';

  r1->rt_num = 0;
  r1->rt_max = 8;
  r1->rt = malloc(r1->rt_max* sizeof(route_table_entry));
  if(r1->rt == NULL){ 
    free(r1->nome);
    free(r1);
    return NULL; 
  }

  r1->link_num= 0;
  r1->link_max= 6;
  r1->p_link = malloc(r1->link_max*sizeof(phy_link));
  if(r1->p_link == NULL){ 
    free(r1->rt);
    free(r1->nome);
    free(r1);
    return NULL; 
  }
  return r1;
}

int rt_resize(Router *r1){
  int new_capacity = r1->rt_max * 2;
  route_table_entry * temp = realloc(r1->rt,new_capacity * sizeof(route_table_entry));
  if (temp == NULL){
    printf("Error: Out of memory! Old data is still safe.\n");
    return 0;
  }
  r1->rt = temp;
  r1->rt_max = new_capacity;
  return 1;
}

int phy_link_resize(Router* r1){
  int new_capacity = r1->link_max * 2;
  phy_link * temp = realloc(r1->p_link,new_capacity * sizeof(phy_link));
  if (temp == NULL){
    printf("Error: Out of memory! Old data is still safe.\n");
    return 0;
  }
  r1->p_link = temp;
  r1->link_max = new_capacity;
  return 1;
}

void create_phy_path(Router* r1, Router* r2){
  //Path unico
  int verify;
  if(r1->link_num == r1->link_max)
    verify = phy_link_resize(r1);
  if(r2->link_num == r2->link_max)
    verify = phy_link_resize(r2);

  r1->p_link[r1->link_num].neighbor = r2;
  r2->p_link[r2->link_num].neighbor = r1;

  r1->link_num++;
  r2->link_num++;
}


void init_network(Network *net) {
    net->routers = NULL;
    net->router_count = 0;
    net->router_capacity = 0;
    net->links = NULL;
    net->link_count = 0;
    net->link_capacity = 0;
}

void add_router_to_network(Network *net, Router *r) {
    if (net->router_count >= net->router_capacity) {
        int new_cap = net->router_capacity == 0 ? 4 : net->router_capacity * 2;
        Router **temp = realloc(net->routers, new_cap * sizeof(Router*));
        if (!temp) {
            printf("Erro: memória insuficiente para adicionar roteador.\n");
            return;
        }
        net->routers = temp;
        net->router_capacity = new_cap;
    }
    net->routers[net->router_count++] = r;
}

void add_phy_link_by_indices(Network *net, int idx1, int idx2) {
    if (idx1 < 0 || idx1 >= net->router_count || idx2 < 0 || idx2 >= net->router_count) {
        printf("Índices inválidos.\n");
        return;
    }
    Router *r1 = net->routers[idx1];
    Router *r2 = net->routers[idx2];
    create_phy_path(r1, r2);
    printf("Link físico criado entre %s e %s.\n", r1->nome, r2->nome);
}

void build_links(Network *net) {
    if (net->links != NULL) {
        for (int i = 0; i < net->link_count; i++)
            free(net->links[i]);
        free(net->links);
        net->links = NULL;
        net->link_count = 0;
        net->link_capacity = 0;
    }

    // Conta quantos links únicos serão criados (A<B)
    int unique_links = 0;
    for (int i = 0; i < net->router_count; i++) {
        Router *r = net->routers[i];
        for (int j = 0; j < r->link_num; j++) {
            Router *neighbor = r->p_link[j].neighbor;
            if (r->nome < neighbor->nome) unique_links++;
        }
    }

    if (unique_links == 0) return;

    net->links = malloc(unique_links * sizeof(Net_link*));
    net->link_capacity = unique_links;
    net->link_count = 0;

    for (int i = 0; i < net->router_count; i++) {
        Router *r = net->routers[i];
        for (int j = 0; j < r->link_num; j++) {
            Router *neighbor = r->p_link[j].neighbor;
            if (r->nome < neighbor->nome) {
                Net_link *l = malloc(sizeof(Net_link));
                l->a = r;
                l->b = neighbor;
                l->cost = rand() % 255;
                net->links[net->link_count++] = l;
            }
        }
    }
    printf("Links de rede criados com custos aleatórios.\n");
}

void display_topology(Network *net) {
    if (net->router_count == 0) {
        printf("Nenhum roteador na rede.\n");
        return;
    }
    printf("\n=== ROTEADORES ===\n");
    for (int i = 0; i < net->router_count; i++) {
        Router *r = net->routers[i];
        printf("%d: %s (%s)\n", i, r->nome, r->endereco);
    }

    printf("\n=== LINKS FÍSICOS ===\n");
    for (int i = 0; i < net->router_count; i++) {
        Router *r = net->routers[i];
        for (int j = 0; j < r->link_num; j++) {
            Router *nb = r->p_link[j].neighbor;
            if (r->nome < nb->nome) {
                printf("%s <--> %s\n", r->nome, nb->nome);
            }
        }
    }

    if (net->links != NULL && net->link_count > 0) {
        printf("\n=== LINKS DE REDE (COM CUSTO) ===\n");
        for (int i = 0; i < net->link_count; i++) {
            Net_link *l = net->links[i];
            printf("%s --%d--> %s\n", l->a->nome, l->cost, l->b->nome);
        }
    }
}

// Retorna o índice do próximo salto a partir da origem para um destino
static int get_next_hop_index(SPF_Result *spf, int src_idx, int dest_idx) {
    if (src_idx == dest_idx) return -1;
    int current = dest_idx;
    while (spf->prev[current] != src_idx) {
        current = spf->prev[current];
        if (current == -1) return -1;
    }
    return current;
}

void build_routing_tables(Network *net) {
      if (net == NULL || net->router_count == 0) return;
  
      for (int src = 0; src < net->router_count; src++) {
          Router *source_router = net->routers[src];
          SPF_Result *spf = dijkstra(net, source_router);
          if (spf == NULL) {
              printf("Erro ao executar Dijkstra para %s\n", source_router->nome);
              continue;
          }
  
          source_router->rt_num = 0;
  
          for (int dest = 0; dest < net->router_count; dest++) {
              if (dest == src) continue;
  
              int next_idx = get_next_hop_index(spf, src, dest);
              if (next_idx == -1) continue;
  
              Router *next_hop_router = net->routers[next_idx];
              Router *dest_router = net->routers[dest];
  
              if (source_router->rt_num >= source_router->rt_max) {
                  if (!rt_resize(source_router)) {
                      printf("Falha ao redimensionar tabela de %s\n", source_router->nome);
                      break;
                  }
              }
  
              route_table_entry *entry = &source_router->rt[source_router->rt_num];
              better_strncpy(entry->rede_dest, dest_router->endereco, END_SIZE);
              better_strncpy(entry->mask, "255.255.255.255", END_SIZE);
              better_strncpy(entry->next_hop, next_hop_router->endereco, END_SIZE);
              entry->cost = spf->dist[dest];
  
              source_router->rt_num++;
          }
  
          free_spf_result(spf);
      }
  }



void recalc_routes(Network *net) {
    if (net->router_count == 0) {
        printf("Nenhum roteador para calcular rotas.\n");
        return;
    }
    build_routing_tables(net);
    printf("Tabelas de roteamento recalculadas.\n");
}

void display_routing_tables(Network *net) {
    for (int i = 0; i < net->router_count; i++) {
        Router *r = net->routers[i];
        printf("\n=== TABELA DE ROTEAMENTO: %s (%s) ===\n", r->nome, r->endereco);
        if (r->rt_num == 0) {
            printf("  (vazia)\n");
        } else {
            printf("  Rede Destino       Máscara           Next Hop           Custo\n");
            for (int j = 0; j < r->rt_num; j++) {
                route_table_entry *e = &r->rt[j];
                printf("  %-16s %-16s %-16s %d\n", e->rede_dest, e->mask, e->next_hop, e->cost);
            }
        }
    }
}

void free_network(Network *net) {
    for (int i = 0; i < net->router_count; i++) {
        Router *r = net->routers[i];
        free(r->rt);
        free(r->p_link);
        free(r->nome);
        free(r);
    }
    free(net->routers);
    for (int i = 0; i < net->link_count; i++) {
        free(net->links[i]);
    }
    free(net->links);
    init_network(net);
}

void delete_router(Network *net, int idx) {
    if (net->router_count == 0) {
        printf("Nenhum roteador na rede.\n");
        return;
    }

    if (idx < 0 || idx >= net->router_count) {
        printf("Índice inválido.\n");
        return;
    }

    Router *r = net->routers[idx];

    for (int i = 0; i < r->link_num; i++) {
        Router *neighbor = r->p_link[i].neighbor;
        for (int j = 0; j < neighbor->link_num; j++) {
            if (neighbor->p_link[j].neighbor == r) {
                for (int k = j; k < neighbor->link_num - 1; k++) {
                    neighbor->p_link[k] = neighbor->p_link[k + 1];
                }
                neighbor->link_num--;
                break;
            }
        }
    }

    int i = 0;
    while (i < net->link_count) {
        if (net->links[i]->a == r || net->links[i]->b == r) {
            free(net->links[i]);
            for (int j = i; j < net->link_count - 1; j++) {
                net->links[j] = net->links[j + 1];
            }
            net->link_count--;
        } else {
            i++;
        }
    }

    if (r->nome != NULL) free(r->nome);
    if (r->rt != NULL) free(r->rt);
    if (r->p_link != NULL) free(r->p_link);
    free(r);

    for (int j = idx; j < net->router_count - 1; j++) {
        net->routers[j] = net->routers[j+1];
    }
    net->routers[net->router_count - 1] = NULL;
    net->router_count--;
}


int table_menu(Network *topTopologia){
    int opt;
    if(topTopologia->router_count <=0)
      init_network(topTopologia);
    srand(42); // Seed pra ter um padrao.

    do {
        printf("\n===== MENU DE ROTEAMENTO =====\n");
        printf("1. Criar roteador\n");
        printf("2. Visualizar topologia\n");
        printf("3. Criar link físico entre roteadores\n");
        printf("4. Criar links de rede (Net Links) a partir dos links físicos\n");
        printf("5. Recalcular tabelas de roteamento (Dijkstra)\n");
        printf("6. Exibir tabelas de roteamento\n");
        printf("7. Deletar Roteador\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opt);
        getchar();

        switch(opt) {
            case 1: {
                char endereco[END_SIZE], nome[50], interface[6];
                printf("Endereço IP (ex: 192.168.1.1): ");
                fgets(endereco, END_SIZE, stdin);
                endereco[strcspn(endereco, "\n")] = '\0';
                printf("Nome do roteador: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                printf("Interface (ex: eth0): ");
                fgets(interface, 6, stdin);
                interface[strcspn(interface, "\n")] = '\0';
                Router *r = create_router(endereco, nome, interface);
                if (r) {
                    add_router_to_network(topTopologia, r);
                    printf("Roteador %s criado.\n", nome);
                } else {
                    printf("Erro ao criar roteador.\n");
                }
                break;
            }
            case 2:
                display_topology(topTopologia);
                break;
            case 3: {
                if (topTopologia->router_count < 2) {
                    printf("É necessário pelo menos dois roteadores.\n");
                    break;
                }
                display_topology(topTopologia);
                int idx1, idx2;
                printf("Índice do primeiro roteador: ");
                scanf("%d", &idx1);
                printf("Índice do segundo roteador: ");
                scanf("%d", &idx2);
                add_phy_link_by_indices(topTopologia, idx1, idx2);
                break;
            }
            case 4:
                build_links(topTopologia);
                break;
            case 5:
                if (topTopologia->link_count == 0 && topTopologia->router_count > 0) {
                    printf("Ainda não existem links de rede. Execute a opção 4 primeiro.\n");
                } else {
                    recalc_routes(topTopologia);
                }
                break;
            case 6:
                display_routing_tables(topTopologia);
                break;
            case 7:
                if (topTopologia->router_count == 0) {
                    printf("É necessário pelo menos um roteador.\n");
                    break;
                }
                printf("Qual roteador deseja excluir?");
                display_topology(topTopologia);
                int idx;
                scanf("%d",&idx);
                delete_router(topTopologia,idx);
                printf("Roteador deletado com sucesso"); 
                break;

            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opt != 0);
    return 1;
}
