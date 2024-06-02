#ifndef UTIL_H
#define UTIL_H

#include "../car_control/car_control.h"

Direction get_dir(MinimaxNode prev_node, MinimaxNode next_node);
Turn get_turn(Direction prev_dir, Direction next_dir);

#endif