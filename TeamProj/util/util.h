#ifndef UTIL_H
#define UTIL_H

#include "../car_control/car_control.h"
#include "../server/server.h"
#include "../pathfind/pathfind.h"

int get_score(DGIST *dgist, PathfindNode node);
Direction get_dir(PathfindNode prev_node, PathfindNode next_node);
Turn get_turn(Direction prev_dir, Direction next_dir);
void print_dir(Direction prev_dir, Direction next_dir, Turn turn);
void* print_map(void* arg);
void print_player(void* arg); 

#endif