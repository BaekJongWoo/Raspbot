#include <stdio.h>

#include "../car_control/car_control.h"
#include "../pathfind/pathfind.h"
#include "../server/server.h"

int get_score(DGIST *dgist, PathfindNode node){
    Item tmpItem = (dgist->map[node.row][node.col]).item;
    switch (tmpItem.status) {
        case nothing:
            return 0;
        case item:
            return tmpItem.score;
        case trap:
            return -8;
    }
    return 0;
}

Direction get_dir(PathfindNode prev_node, PathfindNode next_node){
    if (prev_node.row < next_node.row) return down;
    if (prev_node.row > next_node.row) return up;
    if (prev_node.col < next_node.col) return right;
    if (prev_node.col > next_node.col) return left;

    return unknown;
}

Turn get_turn(Direction prev_dir, Direction next_dir){
    switch((prev_dir - next_dir + 4) % 4){
        case 0:
            return go_straight;
        case 1:
            return turn_left;
        case 2:
            return turn_back;
        case 3:
            return turn_right;
    }
    return go_straight;
}

void print_dir(Direction prev_dir, Direction next_dir, Turn turn){
    printf("prev_dir: ");
    switch(prev_dir){
        case up:
            printf("(%d:up)", up);
            break;
        case right:
            printf("(%d:right)", right);
            break;
        case down:
            printf("(%d:down)", down);
            break;
        case left:
            printf("(%d:left)", left);
            break;
    }
    printf(" next_dir: ");
    switch(next_dir){
        case up:
            printf("(%d:up)", up);
            break;
        case right:
            printf("(%d:right)", right);
            break;
        case down:
            printf("(%d:down)", down);
            break;
        case left:
            printf("(%d:left)", left);
            break;
    }
    printf(" So: ");
    switch(turn){
        case go_straight:
            printf("go straight");
            break;
        case turn_back:
            printf("turn back");
            break;
        case turn_right:
            printf("turn right");
            break;
        case turn_left:
            printf("turn left");
            break;
    }
    printf("\n");
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