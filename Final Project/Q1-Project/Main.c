#include <stdlib.h>
#include <time.h>
#include "HeaderQ1.h"
#include "HeaderTree.h"
#include "HeaderAuxiliary.h"
#include "HeaderListMoves.h"
#include "HeaderListOfLists.h"
#include "HeaderGeneral.h"
#include "HeaderPlay.h"


void main()
{
	Player player;
	Board board;

	generateStartingBoard(board);
	
	srand((unsigned int)time(NULL));
	randomizeStartingPlayer(&player);

	PlayGame(board, player);

	// FREEEEEEDOM!!!!!!!!!! not from the project. we need to free stuff
}//main