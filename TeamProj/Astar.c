#include "../include/Astar.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <math.h>

int rowNum[] = {-1, 0, 0, 1};
int colNum[] = {0, -1, 1, 0};

bool isDestination(int row, int col, int dest_row, int dest_col) {
    return (row == dest_row && col == dest_col);
}

bool isValid(int row, int col) {
    return (row >= 0) && (row < MAP_ROW) && (col >= 0) && (col < MAP_COL);
}

bool isUnBlocked(DGIST *map, int row, int col) {
    return map->map[row][col].item.status != trap;
}

double calculateHValue(int row, int col, int dest_row, int dest_col) {
    return abs(row - dest_row) + abs(col - dest_col);
}

void tracePath(Cell cellDetails[MAP_ROW][MAP_COL], int dest_row, int dest_col) {
    printf("The path is: ");
    int row = dest_row;
    int col = dest_col;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
        printf("(%d,%d) <- ", row, col);
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
    printf("(%d,%d)\n", row, col);
}

void aStarSearch(DGIST *map, int start_row, int start_col, int dest_row, int dest_col, ClientAction *clientAction) {
    if (!isValid(start_row, start_col) || !isValid(dest_row, dest_col)) {
        printf("Invalid start or destination\n");
        return;
    }

    if (!isUnBlocked(map, start_row, start_col) || !isUnBlocked(map, dest_row, dest_col)) {
        printf("Blocked start or destination\n");
        return;
    }

    if (isDestination(start_row, start_col, dest_row, dest_col)) {
        printf("Already at the destination\n");
        return;
    }

    bool closedList[MAP_ROW][MAP_COL];
    memset(closedList, false, sizeof(closedList));

    Cell cellDetails[MAP_ROW][MAP_COL];
    int i, j;

    for (i = 0; i < MAP_ROW; i++) {
        for (j = 0; j < MAP_COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    i = start_row, j = start_col;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    typedef struct {
        double f;
        int i, j;
    } Node;

    Node openList[MAP_ROW * MAP_COL];
    int openListSize = 0;

    openList[openListSize].f = 0.0;
    openList[openListSize].i = i;
    openList[openListSize].j = j;
    openListSize++;

    bool foundDest = false;

    while (openListSize > 0) {
        Node node = openList[0];
        openList[0] = openList[--openListSize];

        int i = node.i;
        int j = node.j;
        closedList[i][j] = true;

        for (int d = 0; d < 4; d++) {
            int new_i = i + rowNum[d];
            int new_j = j + colNum[d];

            if (isValid(new_i, new_j)) {
                if (isDestination(new_i, new_j, dest_row, dest_col)) {
                    cellDetails[new_i][new_j].parent_i = i;
                    cellDetails[new_i][new_j].parent_j = j;
                    tracePath(cellDetails, dest_row, dest_col);
                    foundDest = true;
                    clientAction->row = dest_row;
                    clientAction->col = dest_col;
                    clientAction->action = move; // 혹은 setBomb, 전략에 따라 선택
                    return;
                } else if (!closedList[new_i][new_j] && isUnBlocked(map, new_i, new_j)) {
                    double gNew = cellDetails[i][j].g + 1.0;
                    double hNew = calculateHValue(new_i, new_j, dest_row, dest_col);
                    double fNew = gNew + hNew;

                    if (cellDetails[new_i][new_j].f == FLT_MAX || cellDetails[new_i][new_j].f > fNew) {
                        openList[openListSize].f = fNew;
                        openList[openListSize].i = new_i;
                        openList[openListSize].j = new_j;
                        openListSize++;

                        cellDetails[new_i][new_j].f = fNew;
                        cellDetails[new_i][new_j].g = gNew;
                        cellDetails[new_i][new_j].h = hNew;
                        cellDetails[new_i][new_j].parent_i = i;
                        cellDetails[new_i][new_j].parent_j = j;
                    }
                }
            }
        }
    }

    if (!foundDest) {
        printf("Failed to find the destination\n");
    }
}
