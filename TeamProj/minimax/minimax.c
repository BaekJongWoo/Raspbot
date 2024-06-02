#include "minimax.h"
#include <stdlib.h>
#include <stdio.h>

Neighbors get_neighbors(MinimaxNode node) {
    Neighbors neighbors;
    neighbors.count = 0;
    neighbors.nodes = (MinimaxNode *)malloc(4 * sizeof(MinimaxNode));
    
    if (node.row > 0) neighbors.nodes[neighbors.count++] = (MinimaxNode) {node.row - 1, node.col};
    if (node.row < MAP_ROW - 1) neighbors.nodes[neighbors.count++] = (MinimaxNode) {node.row + 1, node.col};
    if (node.col > 0) neighbors.nodes[neighbors.count++] = (MinimaxNode) {node.row, node.col - 1};
    if (node.col < MAP_COL - 1) neighbors.nodes[neighbors.count++] = (MinimaxNode) {node.row, node.col + 1};

    return neighbors;
}

int minimax(DGIST *map, MinimaxNode player_node, MinimaxNode opponent_node, int depth, int maximizing_player) {
    Neighbors neighbors;
    MinimaxNode neighbor;
    int max_eval, min_eval, eval, i;
    int score;

    if (depth == 0){
        return map->map[player_node.row][player_node.col].item.score;
    }
    else{
        if (maximizing_player){
            max_eval = -INF;
            neighbors = get_neighbors(player_node);

            for (int i=0; i<neighbors.count; i++){
                neighbor = neighbors.nodes[i];
                score = map->map[neighbor.row][neighbor.col].item.score;
                map->map[neighbor.row][neighbor.col].item.score = 0;

                eval = minimax(map, neighbor, opponent_node, depth-1, 0) + score; // 해당 노드로 이동하였을 경우의 기댔값
                printf("eval of {%d, %d} : %d\n", neighbor.row, neighbor.col, eval);
                max_eval = eval > max_eval ? eval : max_eval; // 기댔값 중 최대치를 가지는 노드로 이동

                map->map[neighbor.row][neighbor.col].item.score = score;
            }

            free(neighbors.nodes);
            return max_eval;
        }
        else{
            min_eval = INF;
            neighbors = get_neighbors(opponent_node);

            for (int i=0; i<neighbors.count; i++){
                neighbor = neighbors.nodes[i];
                score = map->map[neighbor.row][neighbor.col].item.score;
                map->map[neighbor.row][neighbor.col].item.score = 0;

                eval = minimax(map, player_node, neighbor, depth-1, 1) - score; // 최소화 플레이어가 최대화 플레이어 차례로 넘김
                printf("eval of {%d, %d} : %d\n", neighbor.row, neighbor.col, eval);
                min_eval = eval < min_eval ? eval : min_eval; // 기댔값 중 최대치를 가지는 노드로 이동

                map->map[neighbor.row][neighbor.col].item.score = score;
            }

            free(neighbors.nodes);
            return min_eval;
        }
    }
}

MinimaxNode find_best_move(DGIST *map, MinimaxNode player_start, MinimaxNode opponent_start, int depth) {
    int best_eval = -INF, eval;
    int score;
    MinimaxNode best_move = {-1, -1}, neighbor;
    Neighbors neighbors = get_neighbors(player_start);
    
    for (int i = 0; i < neighbors.count; i++) {
        neighbor = neighbors.nodes[i];
        score = map->map[neighbor.row][neighbor.col].item.score;
        map->map[neighbor.row][neighbor.col].item.score = 0;

        eval = minimax(map, neighbor, opponent_start, depth-1, 0) + score;
        printf("eval of {%d, %d} : %d\n", neighbor.row, neighbor.col, eval);

        if (eval > best_eval) {
            best_eval = eval;
            best_move = neighbor;
        }

        map->map[neighbor.row][neighbor.col].item.score = score;
    }

    free(neighbors.nodes);
    return best_move;
}

// Print the map
void* print_map(void* arg) {
    DGIST* dgist = (DGIST*)arg;
    Item tmpItem;

    printf("==========PRINT MAP==========\n");
    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            tmpItem = (dgist->map[i][j]).item;
            switch (tmpItem.status) {
                case nothing:
                    printf("- ");
                    break;
                case item:
                    printf("%d ", tmpItem.score);
                    break;
                case trap:
                    printf("x ");
                    break;
            }
        }
        printf("\n");
    }
    printf("==========PRINT DONE==========\n");

    return NULL;
}

void print_player(void* arg) {
    DGIST* dgist = (DGIST*)arg;
    client_info client;

    printf("==========PRINT PLAYERS==========\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client = dgist->players[i];
        printf("++++++++++Player %d++++++++++\n", i + 1);
        printf("Location: (%d,%d)\n", client.row, client.col);
        printf("Score: %d\n", client.score);
        printf("Bomb: %d\n", client.bomb);
    }
    printf("==========PRINT DONE==========\n");
}
