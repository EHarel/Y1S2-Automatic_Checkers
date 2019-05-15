#include "HeaderQ1.h"
#include "HeaderTree.h"
#include "HeaderAuxiliary.h"
#include "HeaderListMoves.h"

void play1(SingleSourceMovesTree* tr);
void play2(SingleSourceMovesList* lst);

void main()
{
	Board board;
	SingleSourceMovesTree* tr;
	checkersPos src;

	SingleSourceMovesList* lst;

	// generateStartingBoard(board);
	generateEmptyBoard(board);

	src.row = 'A';
	src.col = '2';


	/* ------- Board manipulation for tests: ------- */
	board[0][1] = TOP_PLAYER;		//	A2
	board[1][2] = BOTTOM_PLAYER;	//	B3
	board[2][0] = TOP_PLAYER;		//	C1
//	board[2][2] = TOP_PLAYER;		//	C3
	board[2][4] = TOP_PLAYER;		//	C5
//	board[2][6] = TOP_PLAYER;		//	C7
	board[3][2] = BOTTOM_PLAYER;	//	D3
	board[3][4] = BOTTOM_PLAYER;	//	D5
	board[4][2] = TOP_PLAYER;		//	E3
	board[4][4] = TOP_PLAYER;		//	E5
	board[5][4] = BOTTOM_PLAYER;	//	F5
	board[6][4] = TOP_PLAYER;		//	G5
	board[6][6] = TOP_PLAYER;		//	G6
	board[7][5] = BOTTOM_PLAYER;	//	H6



	printBoard(board);


	tr = FindSingleSourceMoves(board, &src);

	lst = FindSingleSourceOptimalMove(tr);

	if (!tr)
	{
		printf("Error. Tree is empty. \n");
		exit(1);
	}

	//printf("Total captures of B5: %u\n", tr->source->next_move[0]->next_move[1]->total_captures_so_far);
	//printf("Total captures of D3: %u\n", tr->source->next_move[0]->total_captures_so_far);
	//printf("Total captures of B1: %u\n", tr->source->next_move[0]->next_move[0]->total_captures_so_far);


	//play1(tr);

	printf("Now we shall print the longest route of that tree:\n");
	play2(lst);	//	list
}//main


/* ------------------ Bunch of functions to try out scenarios ------------------*/

/* These are just dumb functions for us to set the board in whatever way we want to try different situations. */

void play1(SingleSourceMovesTree* tr)
{
	printf("Board of root:\n");
	printBoard(tr->source->board);

	printf("Movement options based on pos: %c%c\n\n\n", tr->source->pos->row, tr->source->pos->col);

	printf("Pos of root->nextmove[0]:\n");
	printf("Position: %c%c \n", tr->source->next_move[0]->pos->row, tr->source->next_move[0]->pos->col);
	printf("\nBoard of next_move[0]: \n");
	printBoard(tr->source->next_move[0]->board);


	//printf("pos of root->nextmove[0]->nextmove[1]:\n");
	//printf("position: %c%c \n", tr->source->next_move[0]->next_move[1]->pos->row, tr->source->next_move[0]->next_move[1]->pos->col);
	//printf("\nboard of next_move[0]->nextmove[1]: \n");
	//printboard(tr->source->next_move[0]->next_move[1]->board);
	// printf("Total captures so far of root->nextmove[0]->nextmove[1]: %d\n\n", tr->source->next_move[0]->next_move[1]->total_captures_so_far);

	printf("pos of root->nextmove[0]->nextmove[0]:\n");
	printf("position: %c%c \n", tr->source->next_move[0]->next_move[0]->pos->row, tr->source->next_move[0]->next_move[0]->pos->col);
	printf("\nboard of next_move[0]->nextmove[0]: \n");
	printBoard(tr->source->next_move[0]->next_move[0]->board);
	printf("Total captures so far of root->nextmove[0]->nextmove[0]: %d\n\n", tr->source->next_move[0]->next_move[0]->total_captures_so_far);


	printf("Pos of root->nextmove[1]:\n");
	printf("Position: %c%c \n", tr->source->next_move[1]->pos->row, tr->source->next_move[1]->pos->col);
	printf("\nBoard of next_move[1]: \n");
	printBoard(tr->source->next_move[1]->board);

	//printf("Checking the next of root->left->right->next:\n");
	////printf(tr->source->next_move[0]->next_move[1]->board);
	//printNodeData(tr->source->next_move[0]->next_move[1]->next_move[1]);
}

void play2(SingleSourceMovesList* lst)
{
	if (!lst)
	{
		printf("Non-existent list.\n\n");
		return;
	}
	if (!lst->head)
	{
		printf("Empty list.\n\n");
		return;
	}

	printf("Printing the list:\n");
	printList(lst);
	printf("\n");
}//play2


