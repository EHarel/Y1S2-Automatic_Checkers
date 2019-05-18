#pragma once

#include "HeaderSingleMovesTree.h"

/* ----------------------------- Structs ----------------------------- */
typedef struct _SingleSourceMovesListCell {
	checkersPos							*position;
	unsigned short						captures;
	struct _SingleSourceMovesListCell	*next;
}SingleSourceMovesListCell;

typedef struct _SingleSourceMovesList {
	SingleSourceMovesListCell	*head;
	SingleSourceMovesListCell	*tail;
}SingleSourceMovesList;

/* ----------------------------- Function Declarations ----------------------------- */
SingleSourceMovesList *FindSingleSourceOptimalMove(SingleSourceMovesTree *moves_tree);

// List Management:
void makeEmptyList(SingleSourceMovesList* lst);
void addDataToStartOfList(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesList* lst);
void addDataToEndOfList(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesList* lst);
SingleSourceMovesListCell* createListCell(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesListCell* next);
void freeSSMList(SingleSourceMovesList* lst);
