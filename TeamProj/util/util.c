#include "../car_control/car_control.h"
#include "../minimax/minimax.h"

Direction get_dir(MinimaxNode prev_node, MinimaxNode next_node){
    if (prev_node.row < next_node.row) return up;
    if (prev_node.row > next_node.row) return down;
    if (prev_node.col < next_node.col) return right;
    if (prev_node.col > next_node.col) return left;

    return unknown;
}

Turn get_turn(Direction prev_dir, Direction next_dir){
    switch((prev_dir - next_dir + 4) / 4){
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