#include "HeaderPlay.h"

/* ----------------------------- Static Function Declarations ----------------------------- */

static void determineSecondPlayer(Player starting_player, Player* second_player);
static void printTurnMessage(unsigned int turn, Player starting_player);
static void checkGameOver(Board board, BOOL* gameOver);
static void announceWinner(Board board, unsigned int turn, Player starting_player);


/* ----------------------------- Function Implementations ----------------------------- */

void PlayGame(Board board, Player starting_player)
{

	unsigned int turn=0;
	Player second_player, curr_player;
	BOOL gameOver = FALSE;

	determineSecondPlayer(starting_player, &second_player);

	printf("Starting board:\n");
	printBoard(board);
	printf("\n\n");
	while (!gameOver)
	{
		if (turn % 2 == 0)
			curr_player = starting_player;
		else
			curr_player = second_player;

		printTurnMessage(turn, starting_player);
		Turn(board, curr_player);
		printBoard(board);
		printf("\n");
		checkGameOver(board, &gameOver);
		turn++;
	}

	announceWinner(board, turn - 1, starting_player);

}//playGame

static void printTurnMessage(unsigned int turn, Player starting_player)
{
	char curr_player[MAX_SIZE];
	char first_player[MAX_SIZE];
	char second_player[MAX_SIZE];


	if (starting_player == TOP_PLAYER)
	{
		strcpy(first_player, "TOP_PLAYER");
		strcpy(second_player, "BOTTOM_PLAYER");
	}
	else	//	starting player was bottom
	{
		strcpy(first_player, "BOTTOM_PLAYER");
		strcpy(second_player, "TOP_PLAYER");
	}

	if (turn % 2 == 0)
		strcpy(curr_player, first_player);
	else
		strcpy(curr_player, second_player);

	printf("player %s's turn\n", curr_player);
}//printTurnMessage

static void determineSecondPlayer(Player starting_player, Player* second_player)
{
	if (starting_player == TOP_PLAYER)
		*second_player = BOTTOM_PLAYER;
	else
		*second_player = TOP_PLAYER;
}

static void checkGameOver(Board board, BOOL* gameOver)
{
	unsigned short i;

	for (i = 0; i < BOARD_SIZE; i++)
	{
		if (board[0][i] == BOTTOM_PLAYER)
			*gameOver = TRUE;
	}

	for (i = 0; i < BOARD_SIZE; i++)
	{
		if (board[7][i] == TOP_PLAYER)
			*gameOver = TRUE;
	}
}//checkGameOver

static void announceWinner(Board board, unsigned int turn, Player starting_player)
{
	char winner[MAX_SIZE];

	if (turn % 2 == 0)	//	starting player won
	{
		if (starting_player == TOP_PLAYER)
			strcpy(winner, "Top player");
		else
			strcpy(winner, "Bottom player");
	}
	else	//	second player won
	{
		if (starting_player != TOP_PLAYER)
			strcpy(winner, "Top player");
		else
			strcpy(winner, "Bottom player");
	}

	printf("\nGame over!\n%s victory after %d turns. Praise be!\n", winner, turn);
	printBoard(board);
}//announceWinner