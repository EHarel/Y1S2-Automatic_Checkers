#pragma once

#include <stdlib.h>
#include <stdio.h>

/* ----------------------------- Defines ----------------------------- */

#define TOP_PLAYER 'T'
#define BOTTOM_PLAYER 'B'
#define EMPTY ' '

#define BOARD_SIZE	8

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


/* ----------------------------- Typedefs ----------------------------- */
typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];
typedef unsigned char Player;
typedef int BOOL;


/* ----------------------------- Structs ----------------------------- */
typedef struct _checkersPos {
	char	row;
	char	col;
}checkersPos;


/* ----------------------------- Function Declarations ----------------------------- */
void randomizeStartingPlayer(Player* player);
void copyBoard(Board dest, Board src);
void copyPos(checkersPos* dest, checkersPos* src);
void generateStartingBoard(Board board);
void generateEmptyBoard(Board board);
void printBoard(Board board);
void checkAllocation(void* ptr);

//void printList(SingleSourceMovesList* lst);
//void printCellData(SingleSourceMovesListCell* cell);