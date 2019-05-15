#pragma once

#include "HeaderQ1.h"


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
/* This function receives a tree of a certain unit.
It returns the movement path with the largest amount of captures out of all options.
If there are several which are equal, the function chooses one randomly and returns it.
*/

/* --------- List Management Functions --------- */
void makeEmptyList(SingleSourceMovesList* lst);
void addDataToStartOfList(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesList* lst);
void addDataToEndOfList(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesList* lst);

SingleSourceMovesListCell* createListCell(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesListCell* next);
