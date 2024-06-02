#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server/server.h"
#include "client/client.h"
#include "minimax/minimax.h"

DGIST gameState;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    const int server_port = atoi(argv[2]);

    int sock = connect_to_server(server_ip, server_port);
    if (sock < 0) {
        return -1;
    }

    ClientAction clientAction;

    // 게임 상태 초기화
    memset(&gameState, 0, sizeof(DGIST));

    // 플레이어를 (0,0)으로 이동시키는 정보 설정
    clientAction.row = 0;
    clientAction.col = 0;
    clientAction.action = move;

    // 초기 위치 정보를 서버에 전송
    send_action(sock, &clientAction);

    MinimaxNode prev_pos = {0, 0};

    while (1) {
        // 서버로부터 DGIST 구조체 수신
        if (receive_update(sock, &gameState) <= 0) {
            printf("Connection closed or error occurred\n");
            close(sock);
            return -1;
        }

        // 맵 출력
        printf("\nUpdated map from server:\n");
        print_map(&gameState);

        // 플레이어 상태 출력
        print_player(&gameState);

        MinimaxNode player_start, opponent_start;
        // Minimax 알고리즘을 사용하여 다음 위치 선택
        if (gameState.players[0].row == prev_pos.row && gameState.players[0].col == prev_pos.col){
            player_start.row = gameState.players[0].row;
            player_start.col = gameState.players[0].col;
            opponent_start.row = gameState.players[1].row;
            opponent_start.col = gameState.players[1].col;
        } else {
            player_start.row = gameState.players[1].row;
            player_start.col = gameState.players[1].col;
            opponent_start.row = gameState.players[0].row;
            opponent_start.col = gameState.players[0].col;
        }
        
        MinimaxNode best_move = find_best_move(&gameState, player_start, opponent_start, DEPTH);
        prev_pos = best_move;

        clientAction.row = best_move.row;
        clientAction.col = best_move.col;
        clientAction.action = move; // move 액션으로 설정

        // 클라이언트의 액션을 서버에 보냄
        send_action(sock, &clientAction);

        sleep(1); // 주기적으로 행동을 수행하도록 설정 (예: 1초 대기)
    }

    close(sock);
    return 0;
}
