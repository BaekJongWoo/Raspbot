#ifndef PATHFIND_H
#define PATHFIND_H

#include "../server/server.h"

typedef struct {
    int row, col;
} PathfindNode;

typedef struct {
    PathfindNode *nodes;
    int count;
} Neighbors;

PathfindNode find_next_move(DGIST *dgist, PathfindNode current_node, PathfindNode prev_node);

#endif // PATHFIND_H
