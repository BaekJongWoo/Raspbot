#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    ClientAction cAction;
    DGIST dgist;
    char buffer[BUFFER_SIZE] = {0};

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <Port>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    const int server_port = atoi(argv[2]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Connected to the server.\n");

    while (1) {
        // 클라이언트의 액션을 서버에 보냄
        printf("Enter row, col and action (0: move, 1: set bomb): ");
        scanf("%d %d %d", &cAction.row, &cAction.col, &cAction.action);

        send(sock, &cAction, sizeof(ClientAction), 0);
        
        // 서버로부터 DGIST 구조체 수신
        recv(sock, &dgist, sizeof(DGIST), 0);

        // 맵 출력
        printf("Updated map from server:\n");
        for (int i = 0; i < MAP_ROW; i++) {
            for (int j = 0; j < MAP_COL; j++) {
                switch (dgist.map[i][j].item.status) {
                    case nothing:
                        printf("- ");
                        break;
                    case item:
                        printf("%d ", dgist.map[i][j].item.score);
                        break;
                    case trap:
                        printf("x ");
                        break;
                }
            }
            printf("\n");
        }

        // 플레이어 상태 출력
        for (int i = 0; i < MAX_CLIENTS; i++) {
            printf("Player %d: Row %d, Col %d, Score %d, Bomb %d\n",
                   i + 1, dgist.players[i].row, dgist.players[i].col,
                   dgist.players[i].score, dgist.players[i].bomb);
        }
    }

    close(sock);
    return 0;
}
