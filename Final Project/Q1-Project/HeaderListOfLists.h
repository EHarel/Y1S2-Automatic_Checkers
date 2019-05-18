#pragma once

#include "HeaderListMoves.h"
#include "HeaderGameAuxiliary.h"

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

// List management functions:
void makeEmptyMSMList(MultipleSourceMovesList* lst);

void add_SSML_to_end_of_MSML(MultipleSourceMovesList* lstOfMoves, SingleSourceMovesList* SSMList);

void freeMSMList(MultipleSourceMovesList* lstOfMoves);
