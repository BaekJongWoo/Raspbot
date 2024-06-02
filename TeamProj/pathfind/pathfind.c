#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "pathfind.h"
#include "../server/server.h"
#include "../util/util.h"

#define INF 1000

Neighbors get_neighbors(PathfindNode node) {
    Neighbors neighbors;
    neighbors.count = 0;
    neighbors.nodes = (PathfindNode *)malloc(8 * sizeof(PathfindNode));
    
    if (node.row > 0) neighbors.nodes[neighbors.count++] = (PathfindNode) {node.row - 1, node.col};
    if (node.row < MAP_ROW - 1) neighbors.nodes[neighbors.count++] = (PathfindNode) {node.row + 1, node.col};
    if (node.col > 0) neighbors.nodes[neighbors.count++] = (PathfindNode) {node.row, node.col - 1};
    if (node.col < MAP_COL - 1) neighbors.nodes[neighbors.count++] = (PathfindNode) {node.row, node.col + 1};

    return neighbors;
}

float get_value_distance_sum(DGIST *dgist, PathfindNode node) {
    float sum = 0.0;
    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            if (i == node.row && j == node.col) continue;
            int distance = abs(node.row - i) + abs(node.col - j);
            int value = get_score(dgist, (PathfindNode){i, j});
            printf("{%d %d} : %f, dis %f", i, j, (float)value, (float)distance);
            sum += (float)value / (float)distance;
        }
    }
    return sum;
}

float get_value_distance_max(DGIST *dgist, PathfindNode node) {
    float max = 0.0;
    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            int distance = abs(node.row - i) + abs(node.col - j) + 1;
            int value = get_score(dgist, (PathfindNode){i, j});

            float vps = (float)value / (float)distance;
            //printf("{%d %d} vps : %f\n", i, j, vps);
            max = vps > max ? vps : max;
        }
    }
    return max;
}

PathfindNode find_next_move(DGIST *dgist, PathfindNode current_node, PathfindNode prev_node) {
    Neighbors neighbors = get_neighbors(current_node);
    PathfindNode best_move = {-1, -1};
    float best_value = -INF;

    for (int i = 0; i < neighbors.count; i++) {
        PathfindNode neighbor = neighbors.nodes[i];
        if (neighbor.row == prev_node.row && neighbor.col == prev_node.col) {
            continue; // Skip the previous node
        }

        //float value = get_value_distance_sum(dgist, neighbor);
        float value = get_value_distance_max(dgist, neighbor);
        printf("value of {%d %d} : %f\n", neighbor.row, neighbor.col, value);
        if (value > best_value) {
            best_value = value;
            best_move = neighbor;
        }
    }

    free(neighbors.nodes);
    return best_move;
}
