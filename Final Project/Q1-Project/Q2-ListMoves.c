//#include "MotherHeader.h"

#include "HeaderListMoves.h"
#include "HeaderAuxiliary.h"


/* ----------------------------- Static Function Declarations ----------------------------- */

static unsigned short findMaximumCaptures(SingleSourceMovesTreeNode* node);

static void FindSingleSourceOptimalMoveRec(SingleSourceMovesTreeNode* node, SingleSourceMovesList* lst, unsigned short maxCap, BOOL* begunList);


/* ----------------------------- Function Implementations ----------------------------- */

SingleSourceMovesList *FindSingleSourceOptimalMove(SingleSourceMovesTree *moves_tree)
/* This function forms and returns a list of the best route of movement,
determined by the route with the largest amount of captures (if there are a few, it randomizes).
It works by first finding out the largest capture value in the tree,
Then it sends that value down to the recursion which creates the list.
The recursion knows to start creating the list only once it reached a node with said value in its captures.
The recursion randomizes the order of calls at each node, 
ensuring that if there is more than 1 node with said value,
it will be decided randomly from which one we start building the list.
*/
{
	BOOL beganListConstruction = FALSE;
	unsigned short maxCap;	//	maximum capture value in the tree
	SingleSourceMovesList *lst = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkAllocation(lst);
	// FREE LATER ON!

	makeEmptyList(lst);

	if (!moves_tree->source)	//	check if empty tree
	{
		printf("Empty tree. No list to return.\n");
		return NULL;
	}

	maxCap = findMaximumCaptures(moves_tree->source);

	srand(time(NULL));			///	CHECK THIS!!!!!!!!!!!!
	FindSingleSourceOptimalMoveRec(moves_tree->source, lst, maxCap, &beganListConstruction);

	return lst;
}//FindSingleSourceOptimalMove

static unsigned short findMaximumCaptures(SingleSourceMovesTreeNode* node)
{
	unsigned short leftCap=0, rightCap=0;

	if (!node->next_move[0] && !node->next_move[1])		//	node is a leaf
		return node->total_captures_so_far;

	if (node->next_move[0] && node->next_move[1])
	{
		leftCap = findMaximumCaptures(node->next_move[0]);
		rightCap = findMaximumCaptures(node->next_move[1]);
	}
	else if (node->next_move[0])
		leftCap = findMaximumCaptures(node->next_move[0]);
	else
		rightCap = findMaximumCaptures(node->next_move[1]);

	return max(leftCap, rightCap);
}//findMaximumCaptures

static void FindSingleSourceOptimalMoveRec(SingleSourceMovesTreeNode* node, SingleSourceMovesList* lst, unsigned short maxCap, BOOL* begunList)
{
	BOOL beganListLeft = FALSE, beganListRight = FALSE;
	// int val;

	if (!node->next_move[0] && !node->next_move[1])		//	reached a leaf
	{
		if (node->total_captures_so_far < maxCap)		//	not the leaf with longest path
			return;
		addDataToEndOfList(node->pos, node->total_captures_so_far, lst);
		*begunList = TRUE;
		return;
	}

	if(node->next_move[0] && node->next_move[1]) //	both paths exist
	{
		if (rand() % 2 == 1)	//	start next move[1]
		{
			FindSingleSourceOptimalMoveRec(node->next_move[1], lst, maxCap, &beganListRight);
			if (beganListRight)
			{
				addDataToStartOfList(node->pos, node->total_captures_so_far, lst);
				*begunList = TRUE;
				return;
			}
			FindSingleSourceOptimalMoveRec(node->next_move[0], lst, maxCap, &beganListLeft);
			if (beganListLeft)
			{
				addDataToStartOfList(node->pos, node->total_captures_so_far, lst);
				*begunList = TRUE;
				return;
			}
		}
		else	//	random was 1, not 0
		{
			FindSingleSourceOptimalMoveRec(node->next_move[0], lst, maxCap, &beganListLeft);
			if (beganListLeft)
			{
				addDataToStartOfList(node->pos, node->total_captures_so_far, lst);
				*begunList = TRUE;
				return;
			}
			FindSingleSourceOptimalMoveRec(node->next_move[1], lst, maxCap, &beganListRight);
			if (beganListRight)
			{
				addDataToStartOfList(node->pos, node->total_captures_so_far, lst);
				*begunList = TRUE;
				return;
			}
		}
	}
	else if (node->next_move[0])
		FindSingleSourceOptimalMoveRec(node->next_move[0], lst, maxCap, &beganListLeft);
	else
		FindSingleSourceOptimalMoveRec(node->next_move[1], lst, maxCap, &beganListRight);

	if (beganListLeft || beganListRight)
	{
		addDataToStartOfList(node->pos, node->total_captures_so_far, lst);
		*begunList = TRUE;
		return;
	}
}//FindSingleSourceOptimalMoveRec



// ------------------- Regular List Management Functions ------------------- //

void makeEmptyList(SingleSourceMovesList* lst)
{
	lst->head = lst->tail = NULL;
}//makeEmptyList

void addDataToStartOfList(checkersPos* pos, unsigned short total_captures_so_far, SingleSourceMovesList* lst)
{
	SingleSourceMovesListCell* newHead = createListCell(pos, total_captures_so_far, NULL);

	if(!lst->head)	//	empty list
		lst->head=lst->tail=newHead;
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