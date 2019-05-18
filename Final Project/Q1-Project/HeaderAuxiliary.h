#pragma once

// #include "MotherHeader.h"

#include <stdlib.h>
#include "HeaderQ1.h"
#include "HeaderListMoves.h"


/* ----------------------------- Function Declarations ----------------------------- */

void randomizeStartingPlayer(Player* player);
void copyBoard(Board dest, Board src);
void copyPos(checkersPos* dest, checkersPos* src);
void generateStartingBoard(Board board);
void generateEmptyBoard(Board board);
void fillTopBoard(Board board);
void fillBottBoard(Board board);
void fillMidBoard(Board board);
void printBoard(Board board);
void printList(SingleSourceMovesList* lst);
void printCellData(SingleSourceMovesListCell* cell);

void checkAllocation(void* ptr);