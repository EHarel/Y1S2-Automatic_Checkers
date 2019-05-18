//#include "MotherHeader.h"

#include "HeaderAuxiliary.h"

void randomizeStartingPlayer(Player* player)
{
	// srand(time(NULL));

	if (rand() % 2 == 0)
		*player = TOP_PLAYER;
	else
		*player = BOTTOM_PLAYER;
}//randomizeStartingPlayer

void copyBoard(Board dest, Board src)
{
	int i, j;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
			dest[i][j] = src[i][j];
	}//for row
}//copyBoard

void copyPos(checkersPos* dest, checkersPos* src)
{
	dest->col = src->col;
	dest->row = src->row;
}//copyPos

void generateStartingBoard(Board board)
{
	fillTopBoard(board);
	fillMidBoard(board);
	fillBottBoard(board);
}//generateStartingBoard

void generateEmptyBoard(Board board)
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
			board[i][j] = EMPTY;
	}//for i
}//generateStartingBoard

void fillTopBoard(Board board)
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		if (i % 2 == 0)			//	even row
		{
			for (j = 0; j < 7; j = j + 2)
				(board[i][j]) = EMPTY;
			for (j = 1; j < BOARD_SIZE; j = j + 2)
				(board[i][j]) = 'T';
		}
		else					//	odd row
		{
			for (j = 1; j < BOARD_SIZE; j = j + 2)
				(board[i][j]) = EMPTY;
			for (j = 0; j < BOARD_SIZE; j = j + 2)
				(board[i][j]) = 'T';
		}//for col
	}//for row
}
void fillBottBoard(Board board)
{
	int i, j;
	for (i = 5; i < 8; i++)
	{
		if (i % 2 == 0)			//	even row
		{
			for (j = 0; j < BOARD_SIZE; j = j + 2)
				(board[i][j]) = EMPTY;
			for (j = 1; j < BOARD_SIZE; j = j + 2)
				(board[i][j]) = 'B';
		}
		else					//	odd row
		{
			for (j = 1; j < BOARD_SIZE; j = j + 2)
				(board[i][j]) = EMPTY;
			for (j = 0; j < BOARD_SIZE; j = j + 2)
				(board[i][j]) = 'B';
		}//for col
	}//for row
}
void fillMidBoard(Board board)
{
	int i, j;

	for (i = 3; i < 5; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
			(board[i][j]) = EMPTY;
	}//for row
}

void printBoard(Board board)
{
	int i, j;

	if (!board)
	{
		printf("No board to print, jackass. \n");
		return;
	}

	printf("+-+-+-+-+-+-+-+-+-+-+\n");
	printf("+ |1|2|3|4|5|6|7|8| +\n");
	printf("+-+-+-+-+-+-+-+-+-+-+\n");
	for (i = 0; i < BOARD_SIZE; i++)
	{
		unsigned char row = 'A' + i;
		printf("|%c|", row);
		for (j = 0; j < BOARD_SIZE; j++)
		{
			printf("%c|", board[i][j]);
		}//for column
		printf("%c|", row);
		printf("\n+-+-+-+-+-+-+-+-+-+-+\n");
	}//for row
	printf("+ |1|2|3|4|5|6|7|8| +\n");
	printf("+-+-+-+-+-+-+-+-+-+-+\n");
}//printBoard

void printList(SingleSourceMovesList* lst)
{
	SingleSourceMovesListCell* currCell = lst->head;
	int i = 1;

	while (currCell)
	{
		printf("Cell number %d data:\n", i++);
		printCellData(currCell);
		currCell = currCell->next;
	}
	printf("End of list.\n");
}//printList

void printCellData(SingleSourceMovesListCell* cell)
{
	printf("Checkers position - %c%c\n", cell->position->row, cell->position->col);
	printf("Captures: %d\n", cell->captures);
	printf("\n\n");
}//printCellData






void checkAllocation(void* ptr)
{
	if (!ptr)
	{
		printf("Memory allocation failed. Abort, abort. \n");
		exit(1);
	}
}//checkAllocation