#include <stdlib.h>
#include <limits.h>

#include "table_structs.h"

static int get_router_index(Network *net, Router *r){
    for(int i = 0; i < net->router_count; i++){
        if(net->routers[i] == r)
            return i;
    }

    return -1;
}

static int min_distance(
        int *dist,
        int *visited,
        int count){

    int min = INT_MAX;
    int idx = -1;

    for(int i = 0; i < count; i++){

        if(!visited[i] &&
           dist[i] < min){

            min = dist[i];
            idx = i;
        }
    }

    return idx;
}

SPF_Result* dijkstra(
        Network *net,
        Router *source){

    int count = net->router_count;

    SPF_Result *result =
        malloc(sizeof(SPF_Result));

    if(result == NULL)
        return NULL;

    result->count = count;

    result->dist =
        malloc(count * sizeof(int));

    result->prev =
        malloc(count * sizeof(int));

    if(result->dist == NULL ||
       result->prev == NULL){

        free(result->dist);
        free(result->prev);
        free(result);

        return NULL;
    }

    int *visited =
        calloc(count, sizeof(int));

    if(visited == NULL){

        free(result->dist);
        free(result->prev);
        free(result);

        return NULL;
    }

    for(int i = 0; i < count; i++){

        result->dist[i] = INT_MAX;
        result->prev[i] = -1;
    }

    int src =
        get_router_index(net, source);

    result->dist[src] = 0;

    for(int step = 0;
        step < count;
        step++){

        int current =
            min_distance(
                result->dist,
                visited,
                count);

        if(current == -1)
            break;

        visited[current] = 1;

        Router *router =
            net->routers[current];

        for(int i = 0;
            i < net->link_count;
            i++){

            Net_link *link =
                net->links[i];

            Router *neighbor = NULL;

            if(link->a == router)
                neighbor = link->b;

            else if(link->b == router)
                neighbor = link->a;

            else
                continue;

            int n_idx =
                get_router_index(
                    net,
                    neighbor);

            if(visited[n_idx])
                continue;

            if(result->dist[current] ==
               INT_MAX)
                continue;

            int new_cost =
                result->dist[current] +
                link->cost;

            if(new_cost <
               result->dist[n_idx]){

                result->dist[n_idx] =
                    new_cost;

                result->prev[n_idx] =
                    current;
            }
        }
    }

    free(visited);

    return result;
}

void free_spf_result(
        SPF_Result *result){

    if(result == NULL)
        return;

    free(result->dist);
    free(result->prev);
    free(result);
}
