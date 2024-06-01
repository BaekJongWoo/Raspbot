#include <stdio.h>
#include "../include/Astar.h"
#include "server.h" // 서버로부터 주어진 헤더 파일 포함

int main() {
    DGIST gameState;
    // 초기화 및 서버로부터 gameState를 받아오는 로직이 있어야 합니다.

    예시로 맵 초기화 (실제로는 서버로부터 받아와야 함)
    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            gameState.map[i][j].item.status = nothing;
            gameState.map[i][j].item.score = 0;
        }
    }

    // 시작 위치와 목적지 설정 (예시로 설정)
    int start_row = 0, start_col = 0;
    int dest_row = 3, dest_col = 3;

    ClientAction clientAction;
    aStarSearch(&gameState, start_row, start_col, dest_row, dest_col, &clientAction);

    // 결과를 서버에 전송하는 로직이 있어야 합니다.
    printf("Next Action: Move to (%d, %d) with action %d\n", clientAction.row, clientAction.col, clientAction.action);

    return 0;
}
