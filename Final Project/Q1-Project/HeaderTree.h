#pragma once

//#include "MotherHeader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HeaderQ1.h"

/* ----------------------------- Defines ----------------------------- */

#define TOP_PLAYER 'T'
#define BOTTOM_PLAYER 'B'
#define BOARD_SIZE	8
#define EMPTY ' '


/* ----------------------------- Typedefs ----------------------------- */

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];


/* ----------------------------- Structs ----------------------------- */

typedef struct _checkersPos {
	char	row;
	char	col;
}checkersPos;

typedef struct _SingleSourceMovesTreeNode {
	Board			board;								//	a matrix of chars
	checkersPos		*pos;								//	a location in the board
	unsigned short	total_captures_so_far;				//	number of captures so far
	struct _SingleSourceMovesTreeNode *next_move[2];	//	movement destinations
}SingleSourceMovesTreeNode;

typedef struct _SingleSourceMovesTree {
	SingleSourceMovesTreeNode	*source;
}SingleSourceMovesTree;


/* ----------------------------- Function Declarations ----------------------------- */

SingleSourceMovesTreeNode* createMovesTreeNode(Board board, checkersPos *pos, unsigned short total_captures_so_far, SingleSourceMovesTreeNode* left, SingleSourceMovesTreeNode* right);
void printNodeData(SingleSourceMovesTreeNode* node);

