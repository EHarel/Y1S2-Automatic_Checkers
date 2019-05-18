#pragma once

#include "HeaderQ1.h"
#include "HeaderTree.h"
#include "HeaderListMoves.h"
#include "HeaderListOfLists.h"


void Turn(Board board, Player player);
/* This function receives a board and a player.
It performs the best move out of all possible moves in the board of that player
If the move includes captures, the function removes from the board the captured units.
*/