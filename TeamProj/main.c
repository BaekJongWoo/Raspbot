#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server/server.h"
#include "client/client.h"
#include "car_control/car_control.h"
#include "minimax/minimax.h"
#include "util/util.h"
#include "qr/qr.h"

int main(int argc, char *argv[]) {    
    DGIST gameState;
    MinimaxNode player_start, opponent_start;
    MinimaxNode prev_node, next_node;
    Direction prev_dir, next_dir;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <Port> <PlayerID>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    const int server_port = atoi(argv[2]);
    const int player_id = atoi(argv[3]);

    QRCodeScanner* scanner = createQRCodeScanner();
    if (!initializeQRCodeScanner(scanner)) {
        return -1;
    }

    int sock = connect_to_server(server_ip, server_port);
    if (sock < 0) {
        destroyQRCodeScanner(scanner);
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

    if (player_id == 0){
        prev_node = (MinimaxNode){0,0};
        prev_dir = right;
    }
    else{
        prev_node = (MinimaxNode){4,4};
        prev_dir = left;
    }

    while (1) {
        // 서버로부터 DGIST 구조체 수신
        if (receive_update(sock, &gameState) <= 0) {
            printf("Connection closed or error occurred\n");
            close(sock);
            destroyQRCodeScanner(scanner);
            return -1;
        }

        // 맵 출력
        printf("\nUpdated map from server:\n");
        print_map(&gameState);
        print_player(&gameState);

        // Minimax 알고리즘을 사용하여 다음 위치 선택
        if (player_id == 0){
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
    
        if (opponent_start.row < 0 || opponent_start.row > MAP_ROW - 1 || 
            opponent_start.col < 0 || opponent_start.col > MAP_COL - 1){
            if (player_id == 0){
                opponent_start.row = 4;
                opponent_start.col = 4;
            }
            else{
                opponent_start.row = 0;
                opponent_start.col = 0;
            }
        }
        
        // 다음 노드를 통해 다음 행동 (직진 / U턴 / 좌회전 / 우회전) 결정
        next_node = find_best_move(&gameState, player_start, opponent_start, DEPTH);
        next_dir = get_dir(prev_node, next_node);
        if (next_dir == unknown){
            if (prev_dir == up) next_dir = down;
            if (prev_dir == down) next_dir = up;
            if (prev_dir == right) next_dir = left;
            if (prev_dir == left) next_dir = right;
        }
        Turn turn = get_turn(prev_dir, next_dir);
        // 정보 업데이트
        prev_node = next_node;
        prev_dir = next_dir;

        //1차 직진 이후 끝 탐색 이후 액션
        run_straight(3750);
        slow_detect();
        switch(turn){
            case go_straight:
                car_move_little();
                break;
            case turn_left:
                car_turn_left();
                break;
            case turn_back:
                car_turn_back();
                break;
            case turn_right:
                car_turn_right();
                break;
        }
        // 이동 종료
        car_run(0,0);

        // qr코드 확인 (next_node 와 일치해야 함)
        QRCodeResult qr_result = scanQRCode(scanner);
        if (qr_result.success) {
            printf("QR Code Scan Succeed: {%d, %d}\n", qr_result.digit1, qr_result.digit2);
            if (next_node.row == qr_result.digit1 && next_node.col == qr_result.digit2) {
                printf("Location Correct\n");
            } else {
                printf("Location Wrong!!!!\n");
            }
        }
        
        clientAction.row = next_node.row;
        clientAction.col = next_node.col;
        clientAction.action = move; // move 액션으로 설정

        // 클라이언트의 액션을 서버에 보냄
        send_action(sock, &clientAction);

        sleep(1); // 1초 대기
    }

    close(sock);
    destroyQRCodeScanner(scanner);
    return 0;
}
