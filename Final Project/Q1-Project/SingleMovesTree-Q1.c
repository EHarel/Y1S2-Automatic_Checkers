#define _CRT_SECURE_NO_WARNINGS

#include "HeaderSingleMovesTree.h"

/* Things that are commented out are on death row for deletion.
We might not need them.
*/

/* ----------------------------- Static Function Declarations ----------------------------- */
static SingleSourceMovesTreeNode* FindSingleSourceMovesRec(Board currBoard, checkersPos *currPos, unsigned short totalCaptures, Player player, int verDir, BOOL performedRegularMove, BOOL performedCaptureMove);
// static BOOL checkSlotForPlayerUnit(Board board, checkersPos* pos, Player player);
static void copyBoardsAndPos(Board leftBoard, Board rightBoard, Board srcBoard, checkersPos* leftPos, checkersPos* rightPos, checkersPos* srcPos);
static void validMovement(Player player, Board board, checkersPos* pos, BOOL* validRegularMoveLeft, BOOL* validCaptureMoveLeft, BOOL* validRegularMoveRight, BOOL* validCaptureMoveRight, BOOL performedRegularMove, BOOL performedCaptureMove);
static void validMovementAux(Player player, checkersPos* pos, int verDir, int horDir, Board board, BOOL* validRegularMove, BOOL* validCaptureMove, BOOL performedRegularMove, BOOL performedCaptureMove);
static void movementAmpDeterminer(BOOL validRegularMoveLeft, BOOL validCaptureMoveLeft, BOOL validRegularMoveRight, BOOL validCaptureMoveRight, int verDir, int* movementAmpLeft, int* movementAmpRight);
static void updateFutureBoards(Board leftBoard, Board rightBoard, checkersPos* currPos, BOOL validCaptureMoveLeft, BOOL validCaptureMoveRight, Player player, int verDir, int movementAmpLeft, int movementAmpRight);
static void updateFuturePositions(checkersPos* currPos, checkersPos* leftPos, checkersPos* rightPos, int verDir, int movementAmpLeft, int movementAmpRight);
static void recursiveCalls(SingleSourceMovesTreeNode* node, Board leftBoard, checkersPos* leftPos, Board rightBoard, checkersPos* rightPos, BOOL performedRegularMove, BOOL performedCaptureMove, unsigned short totalCaptures, Player player, int verDir, BOOL validRegularMoveLeft, BOOL validCaptureMoveLeft, BOOL validRegularMoveRight, BOOL validCaptureMoveRight);
static void updateMovementParameters(unsigned short* totalCaptures, BOOL validRegularMove, BOOL validCaptureMove, BOOL* performedRegularMove, BOOL* performedCaptureMove);
static void freeTreeRec(SingleSourceMovesTreeNode* node);


/* ----------------------------- Function Implementations ----------------------------- */
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos *src)
/* This function receives the board with a given state and a slot in the board - src.
If there is a UNIT in the slot, the function returns a binary tree of all the MOVEMENT OPTIONS of that UNIT.
Otherwise (no unit), it returns NULL.
*/
{
	BOOL performedRegularMove = FALSE, performedCaptureMove = FALSE;
	Player player;
	SingleSourceMovesTree* tree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	checkAllocation(tree);

	//if (board[src->row - 'A'][src->col - '1'] == EMPTY)
	//{
	//	printf("No unit in that position.\n");
	//	return NULL;
	//}

	if (board[src->row - 'A'][src->col - '1'] == TOP_PLAYER)
		player = TOP_PLAYER;
	else
		player = BOTTOM_PLAYER;
	
	//if (!checkSlotForPlayerUnit(board, src, player))
	//{
	//	printf("No player unit in %c%c.\n", src->row, src->col);
	//	return NULL;
	//}
	
	//	unit in slot - create a binary tree of all movement options:
	if (player==TOP_PLAYER)
		tree->source = FindSingleSourceMovesRec(board, src, 0, player, DOWN, performedRegularMove, performedCaptureMove);
	else // player==BOTTOM_PLAYER
		tree->source = FindSingleSourceMovesRec(board, src, 0, player, UP, performedRegularMove, performedCaptureMove);

	return tree;
}

/* --------- Tree Management --------- */
SingleSourceMovesTreeNode* createMovesTreeNode(Board board, checkersPos *pos, unsigned short total_captures_so_far, SingleSourceMovesTreeNode* left, SingleSourceMovesTreeNode* right)
/* Basic tree node creation function.
*/
{
	SingleSourceMovesTreeNode* newNode = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	checkAllocation(newNode);

	newNode->pos = (checkersPos*)malloc(sizeof(checkersPos));
	checkAllocation(newNode->pos);

	newNode->total_captures_so_far = total_captures_so_far;
	copyBoard(newNode->board, board);
	copyPos(newNode->pos, pos);

	newNode->next_move[0] = left;
	newNode->next_move[1] = right;

	return newNode;
}//createMovesTreeNode

void freeTree(SingleSourceMovesTree *moves_tree)
/* This is an envelope function to free a tree (save the planet).
It sends the root to a recursive function which handles all the tree nodes.
Then it frees the tree itself.
*/
{
	if (!moves_tree->source)
	{
		printf("Empty tree. Nothing to free.\n\n");
		return;
	}

	freeTreeRec(moves_tree->source);
	free(moves_tree);
}//freeTree


/* --------- Static Functions --------- */
static SingleSourceMovesTreeNode* FindSingleSourceMovesRec(Board currBoard, checkersPos *currPos, unsigned short totalCaptures, Player player, int verDir, BOOL performedRegularMove, BOOL performedCaptureMove)
/* Variable explanations:
	-	movementAmp: movement amplifier. This determines if we move 1 slot or 2, based on if it's a regular move or a capture move.
		We use this variable to help edit the board; the amplifier determines where we'll place the unit - how many columns and rows to advance.
		It's decided by the type of movement - if it's regular move or a capture move.
*/
{
	BOOL validRegularMoveLeft, validCaptureMoveLeft, validRegularMoveRight, validCaptureMoveRight;
	Board leftBoard, rightBoard;
	int movementAmpLeft, movementAmpRight;
	SingleSourceMovesTreeNode* node = createMovesTreeNode(currBoard, currPos, totalCaptures, NULL, NULL);
	checkersPos* leftPos = (checkersPos*)malloc(sizeof(checkersPos));
	checkersPos* rightPos = (checkersPos*)malloc(sizeof(checkersPos));

	checkAllocation(leftPos);
	checkAllocation(rightPos);
	
	copyBoardsAndPos(leftBoard, rightBoard, currBoard, leftPos, rightPos, currPos);
	
	if (performedRegularMove)	//	made a regular move => no more moves to make
	{
		node->next_move[0] = node->next_move[1] = NULL;
		return node;
	}

	validMovement(player, currBoard, currPos, &validRegularMoveLeft, &validCaptureMoveLeft, &validRegularMoveRight, &validCaptureMoveRight, performedRegularMove, performedCaptureMove); //	calculate all possible moves
	movementAmpDeterminer(validRegularMoveLeft, validCaptureMoveLeft, validRegularMoveRight, validCaptureMoveRight, verDir, &movementAmpLeft, &movementAmpRight);
	updateFutureBoards(leftBoard, rightBoard, currPos, validCaptureMoveLeft, validCaptureMoveRight, player, verDir, movementAmpLeft, movementAmpRight);
	updateFuturePositions(currPos, leftPos, rightPos, verDir, movementAmpLeft, movementAmpRight);	//	update positions for recursion call
	
	recursiveCalls(node, leftBoard, leftPos, rightBoard, rightPos, performedRegularMove, performedCaptureMove, totalCaptures, player, verDir, validRegularMoveLeft, validCaptureMoveLeft, validRegularMoveRight, validCaptureMoveRight);

	return node;
}//FindSingleSourceMovesRec

static void copyBoardsAndPos(Board leftBoard, Board rightBoard, Board srcBoard, checkersPos* leftPos, checkersPos* rightPos, checkersPos* srcPos)
{
	copyBoard(leftBoard, srcBoard);
	copyBoard(rightBoard, srcBoard);
	copyPos(leftPos, srcPos);
	copyPos(rightPos, srcPos);
}//copyBoardsAndPos

static void validMovement(Player player, Board board, checkersPos* pos, BOOL* validRegularMoveLeft, BOOL* validCaptureMoveLeft, BOOL* validRegularMoveRight, BOOL* validCaptureMoveRight, BOOL performedRegularMove, BOOL performedCaptureMove)
/* This function checks all possible movements from a given position (pos), for player.

It runs the same set of tests for left and right with minor modifications.
It examines if:
-	movement stays within the board limits
-	movement is to a free slot
-	movement is blocked by ally unit (T unit)
-	slot has enemy unit
-	slot beyond enemy unit stays within board
-	slot beyond enemy unit is free
*/
{
	if (player == TOP_PLAYER)
	{
		validMovementAux(player, pos, DOWN, LEFT, board, validRegularMoveLeft, validCaptureMoveLeft, performedRegularMove, performedCaptureMove);
		validMovementAux(player, pos, DOWN, RIGHT, board, validRegularMoveRight, validCaptureMoveRight, performedRegularMove, performedCaptureMove);
	}
	else	//	bottom player
	{
		validMovementAux(player, pos, UP, LEFT, board, validRegularMoveLeft, validCaptureMoveLeft, performedRegularMove, performedCaptureMove);
		validMovementAux(player, pos, UP, RIGHT, board, validRegularMoveRight, validCaptureMoveRight, performedRegularMove, performedCaptureMove);
	}
}

static void validMovementAux(Player player, checkersPos* pos, int verDir, int horDir, Board board, BOOL* validRegularMove, BOOL* validCaptureMove, BOOL performedRegularMove, BOOL performedCaptureMove)
/* This program checks the movement options of a player in a given direction.
-	horDi => horizontal direction
-	verDir => vertical direction
*/
{
	*validRegularMove = FALSE;	//	initialize both to FALSE because we're negative and don't want movement
	*validCaptureMove = FALSE;	//	most scenarios will lead to FALSE anyway, so this reduces repeating updates

	if (pos->col + horDir <'1' || pos->col + horDir >'8')	//	movement in direction is beyond board columns
		return;
	else	//	movement in direction is within board columns
	{
		if (board[pos->row - 'A' + verDir][pos->col - '1' + horDir] == EMPTY)	//	free slot in direction
		{
			if (performedCaptureMove)
			{
				*validRegularMove = FALSE;
				*validCaptureMove = FALSE;
			}
			else
				*validRegularMove = TRUE;
		}
		else if (board[pos->row - 'A' + verDir][pos->col - '1' + horDir] == player)		//	slot not free & occupied by player's own unit
			return;
		else	//	enemy unit in that slot
		{
			if (pos->col + 2 * horDir<'1' || pos->col + 2 * horDir>'8')	//	movement past enemy is out of board
				return;
			else if (board[pos->row - 'A' + 2 * verDir][pos->col - '1' + 2 * horDir] == EMPTY)	//	slot past enemy is within board and free
				*validCaptureMove = TRUE;
		}
	}
}

static void movementAmpDeterminer(BOOL validRegularMoveLeft, BOOL validCaptureMoveLeft, BOOL validRegularMoveRight, BOOL validCaptureMoveRight, int verDir, int* movementAmpLeft, int* movementAmpRight)
{
	// Left movement:
	if (validCaptureMoveLeft)
		*movementAmpLeft = 2;
	else if (validRegularMoveLeft)
		*movementAmpLeft = 1;
	else
		*movementAmpLeft = 0;

	// Right movement:
	if (validCaptureMoveRight)
		*movementAmpRight = 2;
	else if (validRegularMoveRight)
		*movementAmpRight = 1;
	else
		*movementAmpRight = 0;
}//movementAmpCheck

static void updateFutureBoards(Board leftBoard, Board rightBoard, checkersPos* currPos, BOOL validCaptureMoveLeft, BOOL validCaptureMoveRight, Player player, int verDir, int movementAmpLeft, int movementAmpRight)
/* This function is called from the FindSingleSourceMovesRec function.
It updates the future boards of the left and right nodes, moving the pieces around accordingly.
If there's a capture move, it replaces the unit to be captured with an empty slot instead.
*/
{
	leftBoard[currPos->row - 'A'][currPos->col - '1'] = rightBoard[currPos->row - 'A'][currPos->col - '1'] = EMPTY;	//	where unit would leave from
	leftBoard[currPos->row - 'A' + verDir*movementAmpLeft][currPos->col - '1' - 1 * movementAmpLeft] = player;		//	where this unit will be
	rightBoard[currPos->row - 'A' + verDir*movementAmpRight][currPos->col - '1' + 1 * movementAmpRight] = player;	//	where this unit will be
	
	if (validCaptureMoveLeft)
	{
		if (player==TOP_PLAYER)	//	direction is up the row number
			leftBoard[currPos->row - 'A' + verDir*movementAmpLeft -1][currPos->col - '1' - 1 * movementAmpLeft +1] = EMPTY;
		else					//	bottom player, direction is down the row number
			leftBoard[currPos->row - 'A' + verDir*movementAmpLeft + 1][currPos->col - '1' - 1 * movementAmpLeft + 1] = EMPTY;
	}
	if (validCaptureMoveRight)
	{
		if(player==TOP_PLAYER)	//	direction is up the row number
			rightBoard[currPos->row - 'A' + verDir*movementAmpRight-1][currPos->col - '1' + 1 * movementAmpRight-1] = EMPTY;	//	where this unit will be
		else					//	bottom player, direction is down the row number
			rightBoard[currPos->row - 'A' + verDir*movementAmpRight + 1][currPos->col - '1' + 1 * movementAmpRight - 1] = EMPTY;//	where this unit will be
	}
}//updateFutureBoards

static void updateFuturePositions(checkersPos* currPos, checkersPos* leftPos, checkersPos* rightPos, int verDir, int movementAmpLeft, int movementAmpRight)
/* This function updates the future position of a unit - where it will be after going left, and after going right.
It performs this update based on:
	-	vertical direction (verDir => UP or DOWN, based on Player)
	-	movement amplifier:
		Movement amp is determined by whether or not there's a capture move.
		If there's a capture move, the amplifier is 2 (the unit moves two rows and two columns).
*/
{
	leftPos->row = currPos->row + verDir*movementAmpLeft;
	leftPos->col = currPos->col + -1*movementAmpLeft;

	rightPos->row = currPos->row + verDir*movementAmpRight;
	rightPos->col = currPos->col + movementAmpRight;
}

static void recursiveCalls(SingleSourceMovesTreeNode* node, Board leftBoard, checkersPos* leftPos, Board rightBoard, checkersPos* rightPos, BOOL performedRegularMove, BOOL performedCaptureMove, unsigned short totalCaptures, Player player, int verDir, BOOL validRegularMoveLeft, BOOL validCaptureMoveLeft, BOOL validRegularMoveRight, BOOL validCaptureMoveRight)
/* This function performs the recursive calls and inserts what the recursion returns into node.
The reason these actions are given to an external function to the recursion is only to make the recursion itself more readable,
by not crowding it with many "if-else"s.
*/
{
	unsigned short totalCapLeft = totalCaptures;
	unsigned short totalCapRight = totalCaptures;
	
	if (!validRegularMoveLeft && !validCaptureMoveLeft)
		node->next_move[0] = NULL;
	else	//	we can move left somehow
	{
		updateMovementParameters(&totalCapLeft, validRegularMoveLeft, validCaptureMoveLeft, &performedRegularMove, &performedCaptureMove);
		node->next_move[0] = FindSingleSourceMovesRec(leftBoard, leftPos, totalCapLeft, player, verDir, performedRegularMove, performedCaptureMove);
	}

	if (!validRegularMoveRight && !validCaptureMoveRight)
		node->next_move[1] = NULL;
	else
	{
		updateMovementParameters(&totalCapRight, validRegularMoveRight, validCaptureMoveRight, &performedRegularMove, &performedCaptureMove);
		node->next_move[1] = FindSingleSourceMovesRec(rightBoard, rightPos, totalCapRight, player, verDir, performedRegularMove, performedCaptureMove);
	}
}//recursiveCalls

static void updateMovementParameters(unsigned short* totalCaptures, BOOL validRegularMove, BOOL validCaptureMove, BOOL* performedRegularMove, BOOL* performedCaptureMove)
{
	if (validCaptureMove)
	{
		(*totalCaptures)++;
		*performedRegularMove = FALSE;
		*performedCaptureMove = TRUE;
	}
	else	//	not capture move => regular movement without capture
	{
		*performedRegularMove = TRUE;
		*performedCaptureMove = FALSE;
	}
}//updateMovementParameters

static void freeTreeRec(SingleSourceMovesTreeNode* node)
{
	if (node->next_move[0])
		freeTreeRec(node->next_move[0]);
	if (node->next_move[1])
		freeTreeRec(node->next_move[1]);

	free(node->pos);
	free(node);
}//freeTreeRec

//static BOOL checkSlotForPlayerUnit(Board board, checkersPos* pos, Player player)
//{
//	if (board[pos->row - 'A'][pos->col - '1'] == player)	// player unit in slot
//		return TRUE;
//	else	//	not player unit - either empty or enemy
//		return FALSE;
//}
