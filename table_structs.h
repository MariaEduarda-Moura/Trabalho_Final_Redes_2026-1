#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

// Tamanho do endereço n+1 para facilitar manipulação de strings
#define END_SIZE 17 

// Function Prototypes

typedef struct Router Router;
typedef struct Net_link Net_link;

typedef struct{
  Router *neighbor;
} phy_link;

struct Net_link{
    int cost;
    Router *a;
    Router *b;
};

typedef struct{
  char rede_dest[END_SIZE];
  char mask[END_SIZE];
  char next_hop[END_SIZE];
  int cost;
} route_table_entry;

struct Router{
  char endereco [END_SIZE];
  char* nome;
  char interface [6];
  int rt_num;
  int link_num; 
  int rt_max;
  int link_max;
  route_table_entry * rt; 
  phy_link* p_link; 
};

typedef struct Network{
    Router **routers;
    int router_count;
    int router_capacity;
    Net_link **links;
    int link_count;
    int link_capacity;
} Network;

typedef struct{
    int *dist;
    int *prev;
    int count;
} SPF_Result;

SPF_Result* dijkstra(Network *net, Router *source);

void free_spf_result(SPF_Result *result);

int better_strncpy(char* dest, char* src, int len);
Router* create_router(char * endereco, char * nome , char * interface );
void create_phy_path(Router* r1, Router* r2);
void init_network(Network *net);
void add_router_to_network(Network *net, Router *r);
void add_phy_link_by_indices(Network *net, int idx1, int idx2);
void build_links(Network *net);
void display_topology(Network *net);
void build_routing_tables(Network *net);
void recalc_routes(Network *net);
void display_routing_tables(Network *net);
void free_network(Network *net);
int table_menu(Network *topTopologia);

#endif
