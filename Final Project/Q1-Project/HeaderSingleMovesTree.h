#pragma once

//#include "MotherHeader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HeaderGameAuxiliary.h"

/* ----------------------------- Structs ----------------------------- */
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
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos *src);

// Tree Management:
SingleSourceMovesTreeNode* createMovesTreeNode(Board board, checkersPos *pos, unsigned short total_captures_so_far, SingleSourceMovesTreeNode* left, SingleSourceMovesTreeNode* right);
void freeTree(SingleSourceMovesTree *moves_tree);
