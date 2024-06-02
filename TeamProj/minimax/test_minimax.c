#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../server/server.h"
#include "minimax.h"

#define INITIAL_BOMB 4

// 맵과 플레이어 정보를 초기화하는 함수
void initialize_game_state(DGIST *gameState) {
    memset(gameState, 0, sizeof(DGIST));

    // 예시 맵 설정
    int initial_map[MAP_ROW][MAP_COL] = {
        { 0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0},
        { 4,  0,  0,  0,  0},
        { 0,  2,  0,  0,  0},
        { 0,  0,  3,  2,  2}
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

int main() {
    DGIST gameState;
    initialize_game_state(&gameState);

    MinimaxNode player_start = {gameState.players[0].row, gameState.players[0].col};
    MinimaxNode opponent_start = {gameState.players[1].row, gameState.players[1].col};

    // Minimax 알고리즘을 사용하여 다음 위치 선택
    MinimaxNode best_move = find_best_move(&gameState, player_start, opponent_start, DEPTH);

    printf("Best move for player from (%d, %d) is to (%d, %d)\n", player_start.row, player_start.col, best_move.row, best_move.col);

    // 맵 출력
    print_map(&gameState);

    // 플레이어 상태 출력
    print_player(&gameState);

    return 0;
}
