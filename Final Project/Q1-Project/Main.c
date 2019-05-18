#include <time.h>
#include "HeaderPlay.h"

void main()
{
	Player player;
	Board board;

	generateStartingBoard(board);
	
	srand((unsigned int)time(NULL));
	randomizeStartingPlayer(&player);

	PlayGame(board, player);
}//main