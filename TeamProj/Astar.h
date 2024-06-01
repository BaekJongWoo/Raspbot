#ifndef ASTAR_H
#define ASTAR_H

#include <stdbool.h>
#include "../server.h" // 서버로부터 주어진 헤더 파일 포함

typedef struct {
    int parent_i, parent_j;
    double f, g, h;
} Cell;

extern int rowNum[];
extern int colNum[];

bool isDestination(int row, int col, int dest_row, int dest_col);
bool isValid(int row, int col);
bool isUnBlocked(DGIST *map, int row, int col);
double calculateHValue(int row, int col, int dest_row, int dest_col);
void tracePath(Cell cellDetails[MAP_ROW][MAP_COL], int dest_row, int dest_col);
void aStarSearch(DGIST *map, int start_row, int start_col, int dest_row, int dest_col, ClientAction *clientAction);

#endif // ASTAR_H
