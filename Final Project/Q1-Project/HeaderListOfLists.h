#pragma once

//#include "MotherHeader.h"

#include "HeaderListMoves.h"
#include "HeaderAuxiliary.h"
#include "HeaderListMoves.h"


/* ----------------------------- Structs ----------------------------- */

typedef struct _multipleSourceMovesListCell {
	SingleSourceMovesList				*single_source_moves_list;
	struct _multipleSourceMovesListCell	*next;
}MultipleSourceMovesListCell;

typedef struct _multipleSourceMovesList {
	MultipleSourceMovesListCell			*head;
	MultipleSourceMovesListCell			*tail;
}MultipleSourceMovesList;


/* ----------------------------- Function Declarations ----------------------------- */

MultipleSourceMovesList *FindAllPossiblePlayerMoves(Board board, Player player);
/* This function receives a board and a player.
It returns a list of all the best moves from every position in the board holding a player unit,
and given that the unit can even move.
*/


// List management functions:
void makeEmptyMSMList(MultipleSourceMovesList* lst);

void add_SSML_to_end_of_MSML(MultipleSourceMovesList* lstOfMoves, SingleSourceMovesList* SSMList);

