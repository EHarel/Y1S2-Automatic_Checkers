#define _CRT_SECURE_NO_WARNINGS

#include "HeaderQ1.h"
#include "HeaderTree.h"
#include "HeaderAuxiliary.h"

/* ----------------------------- Static Function Declarations ----------------------------- */



/* ----------------------------- Function Implementations ----------------------------- */

SingleSourceMovesTreeNode* createMovesTreeNode(Board board, checkersPos *pos, unsigned short total_captures_so_far, SingleSourceMovesTreeNode* left, SingleSourceMovesTreeNode* right)
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

void printNodeData(SingleSourceMovesTreeNode* node)
{
	if (!node)
	{
		printf("No node to print. \n\n");
		return;
	}
	printf("The node's board:\n");
	printBoard(node->board);

	printf("\nThe node's position: %c%c\n", node->pos->row, node->pos->col);

	printf("The next pointers: left is %p and right is %p\n", node->next_move[0], node->next_move[1]);
}//printNodeData