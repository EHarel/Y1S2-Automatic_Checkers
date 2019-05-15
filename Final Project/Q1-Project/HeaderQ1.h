#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "HeaderTree.h"


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

typedef unsigned char Player;
typedef int BOOL;

/* ----------------------------- Structs ----------------------------- */



/* ----------------------------- Function Declarations ----------------------------- */

SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos *src);

