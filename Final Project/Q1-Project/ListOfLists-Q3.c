#include "HeaderListOfLists.h"


/* ----------------------------- Static Function Declarations ----------------------------- */
static void FindAllPossiblePlayerMovesAux(MultipleSourceMovesList* lstOfMoves, Board board, int i, int j, Player player);
static MultipleSourceMovesListCell* createMMSMLCell(SingleSourceMovesList* SSMList, MultipleSourceMovesListCell* next);
static void freeMSMLCell(MultipleSourceMovesListCell *cell);


/* ----------------------------- Function Implementations ----------------------------- */
MultipleSourceMovesList *FindAllPossiblePlayerMoves(Board board, Player player)
/* This function receives a board and a player.
It returns a list of all the moves from every position in the board holding a player unit.
*/
{
	int i, j;
	MultipleSourceMovesList* lstOfMoves = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
	checkAllocation(lstOfMoves);

	makeEmptyMSMList(lstOfMoves);

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == player)
				FindAllPossiblePlayerMovesAux(lstOfMoves, board, i, j, player);
		}//for j
	}//for i

	return lstOfMoves;
}//FindAllPossiblePlayerMoves

 // List Management Functions:
void makeEmptyMSMList(MultipleSourceMovesList* lst)
{
	lst->head = lst->tail = NULL;
}

void add_SSML_to_end_of_MSML(MultipleSourceMovesList* lstOfMoves, SingleSourceMovesList* SSMList)
{
	MultipleSourceMovesListCell* res = createMMSMLCell(SSMList, NULL);

	if (!lstOfMoves->head)	//	no head => empty list
		lstOfMoves->head = lstOfMoves->tail = res;
	else
	{
		lstOfMoves->tail->next = res;
		lstOfMoves->tail = res;
	}
}

void freeMSMList(MultipleSourceMovesList* lstOfMoves)
{
	MultipleSourceMovesListCell *currCell, *tempNext;

	if (!lstOfMoves || !lstOfMoves->head)
	{
		printf("Empty list of moves. Nothing to free.\n");
		return;
	}

	currCell = lstOfMoves->head;

	while (currCell)
	{
		tempNext = currCell->next;
		freeMSMLCell(currCell);
		currCell = tempNext;
	}
	
	free(lstOfMoves);
}//free


// Static Functions:
static void FindAllPossiblePlayerMovesAux(MultipleSourceMovesList* lstOfMoves, Board board, int i, int j, Player player)
/* This function does the heavy lifting of FindAllPossiblePlayerMoves.
It receives indexes for a position, and a player.
Then it:
	-	Determines all possible movements of that unit.
	-	Determines which of all the possible movements of that unit is the best path.
Once it finds the best course of movement, it creates a list of it.
The list nodes contain the position of every move, and the amount of captures.
Then it adds that list to a List of Lists, holding all optimal moves of a player.
*/
{
	checkersPos pos;
	SingleSourceMovesTree* SSMTree;
	SingleSourceMovesList* SSMList;

	pos.row = i + 'A';
	pos.col = j + '1';

	SSMTree = FindSingleSourceMoves(board, &pos);		//	first determine all possible routes
	SSMList = FindSingleSourceOptimalMove(SSMTree);		//	then determine which is the best

	add_SSML_to_end_of_MSML(lstOfMoves, SSMList);		//	add the best route to the list of moves
}//FindAllPossiblePlayerMovesAux

static MultipleSourceMovesListCell* createMMSMLCell(SingleSourceMovesList* SSMList, MultipleSourceMovesListCell* next)
{
	MultipleSourceMovesListCell* res = malloc(sizeof(MultipleSourceMovesListCell));
	checkAllocation(res);

	res->single_source_moves_list = SSMList;
	res->next = next;

	return res;
}//createMMSMLCell

static void freeMSMLCell(MultipleSourceMovesListCell *cell)
{
	freeSSMList(cell->single_source_moves_list);
	free(cell);
}//freeMSMLCell