#include "HeaderListOfLists.h"


/* ----------------------------- Static Function Declarations ----------------------------- */

static void FindAllPossiblePlayerMovesAux(MultipleSourceMovesList* lstOfMoves, Board board, int i, int j, Player player);
static MultipleSourceMovesListCell* createMMSMLCell(SingleSourceMovesList* SSMList, MultipleSourceMovesListCell* next);


/* ----------------------------- Function Implementations ----------------------------- */

MultipleSourceMovesList *FindAllPossiblePlayerMoves(Board board, Player player)
{
	int i, j;
	MultipleSourceMovesList* lstOfMoves;

	lstOfMoves = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
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

static void FindAllPossiblePlayerMovesAux(MultipleSourceMovesList* lstOfMoves, Board board, int i, int j, Player player)
{
	checkersPos pos;
	SingleSourceMovesTree* SSMTree;
	SingleSourceMovesList* SSMList;

	pos.row = i + 'A';
	pos.col = j + '1';

	SSMTree=FindSingleSourceMoves(board, &pos);
	SSMList = FindSingleSourceOptimalMove(SSMTree);

	add_SSML_to_end_of_MSML(lstOfMoves, SSMList);
}//FindAllPossiblePlayerMovesAux



// List management functions:
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

static MultipleSourceMovesListCell* createMMSMLCell(SingleSourceMovesList* SSMList, MultipleSourceMovesListCell* next)
{
	MultipleSourceMovesListCell* res = malloc(sizeof(MultipleSourceMovesListCell));
	checkAllocation(res);

	res->single_source_moves_list = SSMList;
	res->next = next;

	return res;
}//createMMSMLCell