#include "HeaderListMoves.h"

/* ----------------------------- Static Function Declarations ----------------------------- */
static unsigned short findMaximumCaptures(SingleSourceMovesTreeNode* node);
static void FindSingleSourceOptimalMoveRec(SingleSourceMovesTreeNode* node, SingleSourceMovesList* lst, unsigned short maxCap, BOOL* begunList);
static void randDetermineLeftRight(unsigned short *firstTurn, unsigned short *secondTurn);


/* ----------------------------- Function Implementations ----------------------------- */
SingleSourceMovesList *FindSingleSourceOptimalMove(SingleSourceMovesTree *moves_tree)
/* This function receives a tree of a certain unit.
It returns a list of the movement path wit hthe largest amount of captures, of all options.
If there are several routes with equal captures, it randomizes.

It works by first finding out the largest capture value in the tree,
Then it sends that value down to the recursion which creates the list.
The recursion knows to start creating the list only once it reached a node with said value in its captures.
The recursion randomizes the order of calls at each node,
ensuring that if there is more than 1 node with said value,
it will be decided randomly from which one we start building the list.
*/
{
	BOOL beganListConstruction = FALSE;
	unsigned short maxCap;					//	maximum capture value in the tree
	SingleSourceMovesList *lst = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	/* This struct holds the data of the best course of movement of a single unit.
	Each unit can have multiple different paths.
	The best one is determined by the amount of captures.
	If there are several paths with equal amount of captures, it is randomized. */
	checkAllocation(lst);

	makeEmptyList(lst);

	if (!moves_tree->source)	//	check if empty tree
	{
		printf("Empty tree. No list to return.\n");
		return NULL;
	}

	maxCap = findMaximumCaptures(moves_tree->source);	//	discover max captures (essentially the height of the tree)
	FindSingleSourceOptimalMoveRec(moves_tree->source, lst, maxCap, &beganListConstruction);	//	find best move, based on max captures
	freeTree(moves_tree);
	/* Once we have the data of the best course of movement stored in a list (positions and captures),
	we have no more use of the tree itself, so we free it. */

	return lst;
}//FindSingleSourceOptimalMove

 /* --------- List Management --------- */
void makeEmptyList(SingleSourceMovesList* lst)
{
	lst->head = lst->tail = NULL;
}//makeEmptyList

SingleSourceMovesListCell* createListCell(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesListCell* next)
{
	SingleSourceMovesListCell* newCell = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
	checkAllocation(newCell);

	newCell->position = (checkersPos*)malloc(sizeof(checkersPos));
	checkAllocation(newCell->position);

	newCell->position->row = pos->row;
	newCell->position->col = pos->col;
	newCell->captures = total_captures_so_far;
	newCell->next = next;

	return newCell;
}//createListCell

void addDataToStartOfList(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesList* lst)
{
	SingleSourceMovesListCell* newHead = createListCell(pos, total_captures_so_far, NULL);

	if (!lst->head)	//	empty list
		lst->head = lst->tail = newHead;
	else
	{
		newHead->next = lst->head;
		lst->head = newHead;
	}
}//addDataToStartOfList

void addDataToEndOfList(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesList* lst)
{
	SingleSourceMovesListCell* newTail = createListCell(pos, total_captures_so_far, NULL);

	if (!lst->head)		//	empty list
		lst->head = lst->tail = newTail;
	else
	{
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}

void freeSSMList(SingleSourceMovesList* lst)
{
	SingleSourceMovesListCell *currCell, *temp;

	if (!lst || !lst->head)
	{
		printf("Empty list. Nothing to free.\n");
		return;
	}

	currCell = lst->head;

	while (currCell)
	{
		temp = currCell->next;

		free(currCell->position);
		free(currCell);
		currCell = temp;
	}

	free(lst);
}//freeSSMList


/* --------- Static Functions --------- */
static unsigned short findMaximumCaptures(SingleSourceMovesTreeNode* node)
/*	This function receives a tree node.
It goes over all possible routes in search of the one with the largest amount of captures.
In essence, it returns the height of the tree (longest route is the one with the most captures).
*/
{
	unsigned short leftCap=0, rightCap=0;

	if (!node->next_move[0] && !node->next_move[1])		//	node is a leaf
		return node->total_captures_so_far;

	//if (node->next_move[0] && node->next_move[1])
	//{
	//	leftCap = findMaximumCaptures(node->next_move[0]);
	//	rightCap = findMaximumCaptures(node->next_move[1]);
	//}

	if (node->next_move[0])
		leftCap = findMaximumCaptures(node->next_move[0]);
	if (node->next_move[1])
		rightCap = findMaximumCaptures(node->next_move[1]);

	return max(leftCap, rightCap);
}//findMaximumCaptures

static void FindSingleSourceOptimalMoveRec(SingleSourceMovesTreeNode* node, SingleSourceMovesList* lst, unsigned short maxCap, BOOL* begunList)
/*	This function receives a tree node, a list of moves and the maximum amount of captures for the tree node.
It creates a list of moves that represents the best route for that unit.
It does so by going over the tree until reaching leaves, and then comparing them to the max captures value (which is already known).
Once it begins constructions of the list from a certain leaf, it uses a BOOL flag to communicate that the recursion may stop.

At each node it randomizes movement left or right
Meaning even if there are several routes with similar amount of captures, they will be chosen randomly.
(Again, once we've found a proper Leaf, we start list construction and then no longer search for another Leaf with the max captures value).
*/
{
	BOOL beganListFirstMove = FALSE, beganListSecondMove = FALSE;
	unsigned short firstMove, secondMove;

	if (!node->next_move[0] && !node->next_move[1])		//	reached a leaf
	{
		if (node->total_captures_so_far < maxCap)		//	not the leaf with longest path
			return;
		addDataToEndOfList(node->pos, node->total_captures_so_far, lst);
		*begunList = TRUE;
		return;
	}

	randDetermineLeftRight(&firstMove, &secondMove);	//	randomly decide whether to start "left" (0) or "right" (1)

	if (node->next_move[firstMove])
	{
		FindSingleSourceOptimalMoveRec(node->next_move[firstMove], lst, maxCap, &beganListFirstMove);
		if (beganListFirstMove)		//	if started list, no point calling second move
		{
			addDataToStartOfList(node->pos, node->total_captures_so_far, lst);
			*begunList = TRUE;
			return;
		}
	}
	if (node->next_move[secondMove])
	{
		FindSingleSourceOptimalMoveRec(node->next_move[secondMove], lst, maxCap, &beganListSecondMove);
		if (beganListSecondMove)
		{
			addDataToStartOfList(node->pos, node->total_captures_so_far, lst);
			*begunList = TRUE;
			return;
		}
	}
}//FindSingleSourceOptimalMoveRec

static void randDetermineLeftRight(unsigned short *firstTurn, unsigned short *secondTurn)
/* This function randomly determines whether we first go left or right in forming the longest path for the list.
*/
{
	if (rand() % 2 == 0)
	{
		*firstTurn = 0;
		*secondTurn = 1;
	}
	else
	{
		*firstTurn = 1;
		*secondTurn = 0;
	}
}