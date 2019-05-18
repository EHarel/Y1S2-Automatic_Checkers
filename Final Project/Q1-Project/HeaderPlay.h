#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "HeaderGeneral.h"

#define MAX_SIZE 30


/* ----------------------------- Function Declarations ----------------------------- */

void PlayGame(Board board, Player starting_player);
/* This function receives a board and a player whose turn it is to play.
The function plays a game where the computer plays aginst itself, starting with starting_player.
The function prints the board of each turn.
Each turn the function prints the player whose turn it is, and the move made for him.
*/