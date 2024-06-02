#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../server/server.h"
#include "pathfind.h"

#define INITIAL_BOMB 4

// 맵과 플레이어 정보를 초기화하는 함수
void initialize_game_state(DGIST *gameState) {
    memset(gameState, 0, sizeof(DGIST));

    // 예시 맵 설정
    int initial_map[MAP_ROW][MAP_COL] = {
        {0, 1, 4, 4, 0},
        {0, 1, 2, 0, 0},
        {4, 0, 0, 0, 3},
        {0, 2, 0, 0, 4},
        {0, 0, 3, 0, 0}
    };

    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            gameState->map[i][j].row = i;
            gameState->map[i][j].col = j;
            if (initial_map[i][j] == -1) {
                gameState->map[i][j].item.status = nothing;
                gameState->map[i][j].item.score = 0;
            } else {
                gameState->map[i][j].item.status = item;
                gameState->map[i][j].item.score = initial_map[i][j];
            }
        }
    }

    // 플레이어 초기 위치 설정
    gameState->players[0].row = 0;
    gameState->players[0].col = 2;
    gameState->players[0].score = 0;
    gameState->players[0].bomb = INITIAL_BOMB;

    gameState->players[1].row = 4;
    gameState->players[1].col = 4;
    gameState->players[1].score = 0;
    gameState->players[1].bomb = INITIAL_BOMB;
}

void print_map(DGIST *dgist) {
    printf("==========MAP==========\n");
    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            printf("%d ", dgist->map[i][j].item.score);
        }
        printf("\n");
    }
    printf("=======================\n");
}

void print_player(DGIST* dgist) {
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

int main() {
    DGIST gameState;
    initialize_game_state(&gameState);

    PathfindNode current_node = {gameState.players[0].row, gameState.players[0].col};
    PathfindNode prev_node = {-1, -1}; // 초기에는 이전 노드가 없으므로 -1로 설정

    // Pathfind 알고리즘을 사용하여 다음 위치 선택
    PathfindNode next_move = find_next_move(&gameState, current_node, prev_node);

    printf("Next move for player from (%d, %d) is to (%d, %d)\n", current_node.row, current_node.col, next_move.row, next_move.col);

    // 맵 출력
    print_map(&gameState);

    // 플레이어 상태 출력
    print_player(&gameState);

    return 0;
}
