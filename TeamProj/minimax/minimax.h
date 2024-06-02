#ifndef MINIMAX_H
#define MINIMAX_H

#include "../server/server.h"

#define DEPTH 5
#define INF 1000

typedef struct {
    int row, col;
} MinimaxNode;

typedef struct {
    MinimaxNode *nodes;
    int count;
} Neighbors;

Neighbors get_neighbors(MinimaxNode node);
int minimax(DGIST *map, MinimaxNode player_node, MinimaxNode opponent_node, int depth, int maximizing_player);
MinimaxNode find_best_move(DGIST *map, MinimaxNode player_start, MinimaxNode opponent_start, int depth);
void* print_map(void* arg); 
void print_player(void* arg);

#endif // MINIMAX_H
