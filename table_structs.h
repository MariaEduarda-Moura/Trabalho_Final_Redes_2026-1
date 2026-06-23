#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

// Tamanho do endereço n+1 para facilitar manipulação de strings
#define END_SIZE 17 

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
  char MAC [18];
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

#endif
