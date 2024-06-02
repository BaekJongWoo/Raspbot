#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server/server.h"
#include "client/client.h"
#include "car_control/car_control.h"
#include "util/util.h"
#include "qr/qr.h"
#include "pathfind/pathfind.h"

int main(int argc, char *argv[]) {    
    DGIST gameState;
    PathfindNode current_node, prev_node, next_node;
    Direction current_dir, next_dir;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <Port> <PlayerID>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    const int server_port = atoi(argv[2]);
    const int player_id = atoi(argv[3]);

    //모터 셋업
    setup();

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
        prev_node = (PathfindNode){0,0};
        current_node = (PathfindNode){0,1};
        current_dir = right;
    }
    else{
        prev_node = (PathfindNode){4,4};
        current_node = (PathfindNode){4,3};
        current_dir = left;
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

        printf("start find\n");
        // 다음 위치 선택
        next_node = find_next_move(&gameState, current_node, prev_node);
        next_dir = get_dir(current_node, next_node);
        if (next_dir == unknown){
            if (current_dir == up) next_dir = down;
            if (current_dir == down) next_dir = up;
            if (current_dir == right) next_dir = left;
            if (current_dir == left) next_dir = right;
        }
        Turn turn = get_turn(current_dir, next_dir);
        printf("prev node: {%d %d} current Node: {%d %d}, next_node: {%d %d}\n", prev_node.row, prev_node.col, 
                current_node.row, current_node.col, next_node.row, next_node.col);
        print_dir(current_dir, next_dir, turn);

        // 정보 업데이트
        prev_node = current_node;
        current_node = next_node;
        current_dir = next_dir;

        //1차 직진 이후 끝 탐색 이후 액션
        printf("run\n");
        run_straight(3500);
        printf("detect\n");
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
    }

    close(sock);
    destroyQRCodeScanner(scanner);
    return 0;
}
