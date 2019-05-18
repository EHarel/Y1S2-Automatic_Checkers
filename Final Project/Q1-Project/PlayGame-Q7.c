#include "HeaderPlay.h"

/* ----------------------------- Static Function Declarations ----------------------------- */

static void determineSecondPlayer(Player starting_player, Player* second_player);
static void printTurnMessage(unsigned int turn, Player starting_player);
static void checkGameOver(Board board, BOOL* gameOver);
static void checkUnitMassacre(Board board, Player player, BOOL* unitExists);

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

	announceWinner(board, turn, starting_player);

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

	printf("%s's turn\n", curr_player);
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
	BOOL topUnits, botUnits;

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

	checkUnitMassacre(board, TOP_PLAYER, &topUnits);
	checkUnitMassacre(board, BOTTOM_PLAYER, &botUnits);

	if (!topUnits || !botUnits)
		*gameOver = TRUE;
}//checkGameOver

static void checkUnitMassacre(Board board, Player player, BOOL* unitExists)
{
	unsigned short i, j;
	BOOL found = FALSE;

	for (i = 0; i < BOARD_SIZE && !found; i++)
	{
		for (j = 0; j < BOARD_SIZE && !found; j++)
		{
			if (board[i][j] == player)
				found = TRUE;
		}//for j
	}//for i

	if (found)
		*unitExists = TRUE;
	else
		*unitExists = FALSE;
}//checkUnitMassacre

static void announceWinner(Board board, unsigned int turn, Player starting_player)
{
	char winner[MAX_SIZE];

	if (turn % 2 == 1)	//	starting player won (the value is increased after the movement, so it starts at 0 and goes to 1 after first player's move)
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

	printf("Game over! %s victory after %d turns. Praise be!\n\n", winner, turn);
	// printBoard(board);
}//announceWinner