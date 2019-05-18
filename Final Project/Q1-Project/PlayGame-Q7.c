#include "HeaderPlay.h"

/* ----------------------------- Static Function Declarations ----------------------------- */
static void determineSecondPlayer(Player starting_player, Player* second_player);
static void printTurnMessage(unsigned int turn, Player starting_player);
static void checkGameOver(Board board, BOOL* gameOver, Player curr_player);
static void checkUnitMassacre(Board board, Player enemy_player, BOOL* enemy_has_units);
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
		checkGameOver(board, &gameOver, curr_player);
		turn++;
	}

	announceWinner(board, turn, curr_player);
}//playGame


 /* --------- Static Functions --------- */
static void determineSecondPlayer(Player starting_player, Player* second_player)
{
	if (starting_player == TOP_PLAYER)
		*second_player = BOTTOM_PLAYER;
	else
		*second_player = TOP_PLAYER;
}

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

static void checkGameOver(Board board, BOOL* gameOver, Player curr_player)
{
	unsigned short i, j;
	Player enemy_player;
	BOOL enemy_has_units;

	if (curr_player == TOP_PLAYER)
	{
		enemy_player = BOTTOM_PLAYER;
		i = 7;
	}
	else
	{
		enemy_player = TOP_PLAYER;
		i = 0;
	}

	for (j = 0; j < BOARD_SIZE; j++)	//	check if current player reached opposite end of board
	{
		if (board[i][j] == curr_player)
			*gameOver = TRUE;
	}

	if (*gameOver == TRUE)
		return;

	checkUnitMassacre(board, enemy_player, &enemy_has_units);

	//checkUnitMassacre(board, TOP_PLAYER, &topUnits);
	//checkUnitMassacre(board, BOTTOM_PLAYER, &botUnits);

	if (!enemy_has_units)
		*gameOver = TRUE;
}//checkGameOver

static void checkUnitMassacre(Board board, Player enemy_player, BOOL* enemy_has_units)
{
	unsigned short i, j;
	BOOL found = FALSE;

	for (i = 0; i < BOARD_SIZE && !found; i++)
	{
		for (j = 0; j < BOARD_SIZE && !found; j++)
		{
			if (board[i][j] == enemy_player)
				found = TRUE;
		}//for j
	}//for i

	if (found)
		*enemy_has_units = TRUE;
	else
		*enemy_has_units = FALSE;
}//checkUnitMassacre

static void announceWinner(Board board, unsigned int turn, Player curr_player)
{
	char winner[MAX_SIZE];

	if (curr_player==TOP_PLAYER)
		strcpy(winner, "Top player");
	else
		strcpy(winner, "Bottom player");


	//if (turn % 2 == 1)	//	starting player won (the value is increased after the movement, so it starts at 0 and goes to 1 after first player's move)
	//{
	//	if (starting_player == TOP_PLAYER)
	//		strcpy(winner, "Top player");
	//	else
	//		strcpy(winner, "Bottom player");
	//}
	//else	//	second player won
	//{
	//	if (starting_player != TOP_PLAYER)
	//		strcpy(winner, "Top player");
	//	else
	//		strcpy(winner, "Bottom player");
	//}

	printf("Game over! %s victory after %d turns. Praise be!\n\n", winner, turn);
	// printBoard(board);
}//announceWinner