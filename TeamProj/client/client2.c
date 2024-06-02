#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "server.h"

#define BUFFER_SIZE 1024

void* receiveUpdates(void* arg) {
    int sock = *((int*)arg);
    DGIST dgist;
    int valRead;

    while (1) {
        valRead = recv(sock, &dgist, sizeof(DGIST), 0);
        if (valRead <= 0) {
            printf("Connection closed or error occurred\n");
            close(sock);
            pthread_exit(NULL);
        }

        // 맵 출력
        printf("\nUpdated map from server:\n");
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
        printf("==========PRINT PLYAERS==========\n");
        for(int i=0; i < MAX_CLIENTS; i++){
            printf("++++++++++Player %d++++++++++\n",i+1);
            printf("Location: (%d,%d)\n",dgist.players[i].row, dgist.players[i].col);
            printf("Score: %d\n",dgist.players[i].score);
            printf("Bomb: %d\n",dgist.players[i].bomb);
        }
        printf("Enter row, col and action (0: move, 1: set bomb): ");
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    ClientAction cAction;
    pthread_t receiveThread;

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

    // Start the thread to receive updates from the server
    if (pthread_create(&receiveThread, NULL, receiveUpdates, &sock) != 0) {
        perror("pthread_create");
        close(sock);
        return -1;
    }

    while (1) {
        // 클라이언트의 액션을 서버에 보냄
        printf("Enter row, col and action (0: move, 1: set bomb): ");
        scanf("%d %d %d", &cAction.row, &cAction.col, &cAction.action);

        send(sock, &cAction, sizeof(ClientAction), 0);
    }

    // This will never be reached in this example, but should be here for completeness
    pthread_join(receiveThread, NULL);
    close(sock);
    return 0;
}
