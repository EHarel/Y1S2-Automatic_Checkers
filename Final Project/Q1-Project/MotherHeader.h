#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "HeaderTree.h"
#include "HeaderQ1.h"
#include "HeaderListMoves.h"
#include "HeaderListMoves.h"
#include "HeaderListOfLists.h"
#include "HeaderAuxiliary.h"

#define TRUE 1
#define FALSE 0

#define LEFT -1
#define RIGHT 1

#define UP -1
#define DOWN 1
/* The reason for these defines being unintuitive is for the calculations in some functions;
If you move UP, the columns in the board matrix decrease (C, D, A).
If you move DOWN, the columns in the board matric increase (A, B, C).
It's unintuitive if we view the board top-to-bottom.
*/

#define TOP_PLAYER 'T'
#define BOTTOM_PLAYER 'B'
#define BOARD_SIZE	8
#define EMPTY ' '

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];




typedef unsigned char Player;
typedef int BOOL;
