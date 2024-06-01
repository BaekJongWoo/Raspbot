
#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <string.h>

#define ROW 5
#define COL 5

typedef struct {
    int parent_i, parent_j;
    double f, g, h;
} Cell;

int rowNum[] = {-1, 0, 0, 1};
int colNum[] = {0, -1, 1, 0};

bool isDestination(int row, int col, int dest_row, int dest_col) {
    return (row == dest_row && col == dest_col);
}

bool isValid(int row, int col) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

bool isUnBlocked(int grid[ROW][COL], int row, int col) {
    return grid[row][col] == 1;
}

double calculateHValue(int row, int col, int dest_row, int dest_col) {
    return abs(row - dest_row) + abs(col - dest_col);
}

void tracePath(Cell cellDetails[ROW][COL], int dest_row, int dest_col) {
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

void aStarSearch(int grid[ROW][COL], int start_row, int start_col, int dest_row, int dest_col) {
    if (!isValid(start_row, start_col) || !isValid(dest_row, dest_col)) {
        printf("Invalid start or destination\n");
        return;
    }

    if (!isUnBlocked(grid, start_row, start_col) || !isUnBlocked(grid, dest_row, dest_col)) {
        printf("Blocked start or destination\n");
        return;
    }

    if (isDestination(start_row, start_col, dest_row, dest_col)) {
        printf("Already at the destination\n");
        return;
    }

    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));

    Cell cellDetails[ROW][COL];
    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
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

    Node openList[ROW * COL];
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
                    return;
                } else if (!closedList[new_i][new_j] && isUnBlocked(grid, new_i, new_j)) {
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

int main() {
    int grid[ROW][COL] = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}
    };

    int start_row = 0, start_col = 0;
    int dest_row = 4, dest_col = 4;

    aStarSearch(grid, start_row, start_col, dest_row, dest_col);

    return 0;
}
