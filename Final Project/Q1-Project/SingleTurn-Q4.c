
// #include "HeaderAuxiliary.h"
#include "HeaderTurn.h"

/* ----------------------------- Static Function Declarations ----------------------------- */
static void updateBoardWithBestTurn(Board board, Player player, SingleSourceMovesList* SSMList);
static void printUnitMovement(MultipleSourceMovesListCell *maxCapturesCell);
static void comparePositionsOfNoCaptureCells(MultipleSourceMovesListCell **currCell, MultipleSourceMovesListCell **maxCapturesCell, MultipleSourceMovesListCell **maxPrev, MultipleSourceMovesListCell *currPrev);
static void updateStartAndEndPositions(checkersPos *startPos, checkersPos *endPos, SingleSourceMovesList* SSMList);
static void checkMovement(checkersPos* startPos, checkersPos* endPos, BOOL* cellUnitMoved);
static void eraseUnitStartPosition(Board board, SingleSourceMovesList* SSMList);
static void placeUnitEndPosition(Board board, Player player, SingleSourceMovesList* SSMList);
static void eraseCapturedUnits(Board board, Player player, SingleSourceMovesList* SSMList);
static int determineVerDir(Player player);
static void updatePositions(char* currRow, char* currCol, char* nextRow, char* nextCol, SingleSourceMovesListCell* currCell);
static int updateHorDir(char currCol, char nextCol);


/* ----------------------------- Function Implementations ----------------------------- */
void Turn(Board board, Player player)
{
	MultipleSourceMovesList* lstOfMoves;
	MultipleSourceMovesListCell *maxCapturesCell, *currCell, *maxPrev, *currPrev;	//	track the prev so we can easily remove from list later on

	lstOfMoves = FindAllPossiblePlayerMoves(board, player);
	/* First we find all the possible moves.
	After that we determine which of all the possible moves is the best (most captures). */

	if (!lstOfMoves)
	{
		printf("No list of moves. No move to perform.\n");
		return;
	}

	maxCapturesCell = currCell = lstOfMoves->head;
	maxPrev = currPrev = NULL;

	while (currCell)
	{
		// Compare capture values in current cell and the max capture cell so far:
		if (currCell->single_source_moves_list->tail->captures > maxCapturesCell->single_source_moves_list->tail->captures)		//	curr cell has higher capture value => switch the pointer to it
			maxCapturesCell = currCell;
		else if (currCell->single_source_moves_list->tail->captures == maxCapturesCell->single_source_moves_list->tail->captures)
		/* Equal amounts of capture => we check if the cells involve regular movement, or remain in place. */
			comparePositionsOfNoCaptureCells(&currCell, &maxCapturesCell, &maxPrev, currPrev);

		currPrev = currCell;
		currCell = currCell->next;
	}   //  continue until list is over

	printUnitMovement(maxCapturesCell);
	updateBoardWithBestTurn(board, player, maxCapturesCell->single_source_moves_list);
	freeMSMList(lstOfMoves);
}//Turn


 /* --------- Static Functions --------- */
static void comparePositionsOfNoCaptureCells(MultipleSourceMovesListCell **currCell, MultipleSourceMovesListCell **maxCapturesCell, MultipleSourceMovesListCell **maxPrev, MultipleSourceMovesListCell *currPrev)
/* We reach this function upon having two cells with no captures. This could mean:
	-	One or both can't move at all.
	-	One or both made regular move.
We need to determine if one moved, both moved or none at all.
If both made a regular move, we randomize where to place the maxCapturesCell pointer.

We keep track of prev so we can easily release the list pointed at by max after using it.
*/
{
	BOOL currCellUnitMoved = FALSE, maxCellUnitMoved = FALSE;

	checkersPos currCellStartPos, currCellEndPos, maxCellStartPos, maxCellEndPos;

	updateStartAndEndPositions(&currCellStartPos, &currCellEndPos, (*currCell)->single_source_moves_list);
	updateStartAndEndPositions(&maxCellStartPos, &maxCellEndPos, (*maxCapturesCell)->single_source_moves_list);
	
	checkMovement(&currCellStartPos, &currCellEndPos, &currCellUnitMoved);
	checkMovement(&maxCellStartPos, &maxCellEndPos, &maxCellUnitMoved);

	if (currCellUnitMoved && maxCellUnitMoved)
		/* Both units moved (meaning, performed regular move in this context) => we randomize to place the max pointer. */
	{
		if (rand() % 2 == 0)
		{
			*maxCapturesCell = *currCell;
			*maxPrev = currPrev;
		}
	}
	else if (currCellUnitMoved && !maxCellUnitMoved)
	{
		*maxCapturesCell = *currCell;
		*maxPrev = currPrev;
	}
}//comparePositionsOfNoCaptureCells

static void updateStartAndEndPositions(checkersPos *startPos, checkersPos *endPos, SingleSourceMovesList* SSMList)
{
	startPos->row = SSMList->head->position->row;
	startPos->col = SSMList->head->position->col;

	endPos->row = SSMList->tail->position->row;
	endPos->col = SSMList->tail->position->col;
}//updatePositions

static void checkMovement(checkersPos* startPos, checkersPos* endPos, BOOL* cellUnitMoved)
{
	if (endPos->row != startPos->row)
		*cellUnitMoved = TRUE;
	else
		*cellUnitMoved = FALSE;
}//checkMovement

static void printUnitMovement(MultipleSourceMovesListCell *maxCapturesCell)
{
	SingleSourceMovesListCell* cell = maxCapturesCell->single_source_moves_list->head;

	printf("%c%c", cell->position->row,cell->position->col);
	cell = cell->next;
	while (cell)
	{
		printf("->%c%c", cell->position->row, cell->position->col);
		cell = cell->next;
	}
	printf("\n");
}

static void updateBoardWithBestTurn(Board board, Player player, SingleSourceMovesList* SSMList)
{

	eraseUnitStartPosition(board, SSMList);
	placeUnitEndPosition(board, player, SSMList);

	if (SSMList->tail->captures>0)	//	captures have been performed => there are enemy units to remove
		eraseCapturedUnits(board, player, SSMList);
}

static void eraseUnitStartPosition(Board board, SingleSourceMovesList* SSMList)
{
	// Copy to variables for readability and comfort:
	char row = SSMList->head->position->row;
	char col = SSMList->head->position->col;

	// Go to that position in the board and place a SPACE there:
	board[row - 'A'][col - '1'] = EMPTY;
}

static void placeUnitEndPosition(Board board, Player player, SingleSourceMovesList* SSMList)
{
	// Copy to variables for readability and comfort:
	char row = SSMList->tail->position->row;
	char col = SSMList->tail->position->col;

	// Go to that position in the board and place PLAYER there:
	board[row - 'A'][col - '1'] = player;
}//placeUnitEndPosition

static void eraseCapturedUnits(Board board, Player player, SingleSourceMovesList* SSMList)
{

	// Find out which direction to go in (DOWN or UP) based on player:
	int verDir = determineVerDir(player);
	int horDir;
	SingleSourceMovesListCell* currCell = SSMList->head;
	char currRow, currCol, nextRow, nextCol;	//	using them just for comfort and readability, not necessity


	while (currCell->next)
	/* Continue as long as we're not at the tail.
	The tail is the final position of this unit and there will be no more enemy units beyond that point to delete.
	*/
	{
		updatePositions(&currRow, &currCol, &nextRow, &nextCol, currCell);
		horDir=updateHorDir(currCol, nextCol);

		board[currRow + verDir-'A'][currCol + horDir-'1'] = EMPTY;

		currCell = currCell->next;
	}

}//eraseCapturedUnits

static int determineVerDir(Player player)
// VerDir => vertical direction, up or down based on player
{
	if (player == TOP_PLAYER)
		return DOWN;
	else		//	bottom player
		return UP;
}

static void updatePositions(char* currRow, char* currCol, char* nextRow, char* nextCol, SingleSourceMovesListCell* currCell)
{
	*currRow = currCell->position->row;
	*currCol = currCell->position->col;

	*nextRow = currCell->next->position->row;
	*nextCol = currCell->next->position->col;
}

static int updateHorDir(char currCol, char nextCol)
{
	if (nextCol > currCol)
		return RIGHT;
	else
		return LEFT;
}
