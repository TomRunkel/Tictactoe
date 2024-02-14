#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>

#define PROGRAM_WIDTH 69

COORD coord = { 0, 0 };
char game[9] = {'1','2','3','4','5','6','7','8','9'};
char messageBuffer[PROGRAM_WIDTH + 1] = "";
int turn = 0;
int startingPlayer = 0;

void gameterminal(int x, int y);
void startmenu(char* outputMessage, int chose);
void startgame(int chose);
void rungame(char* outputMessage, int chose);
void runmenu(void);
void printheader(void);
void printmenu(char* outputMessage, int chose);
void printgame(char* outputMessage);
void printEndscreen(char* outputMessage);
char* centerMessage(char* outputMessage);
void flushInput(void);
void checkVictory(char player, char opponent);
void opponentsMove(char player, char opponent);
int checkPossibleVictory(char player, char opponent);

int main(void)
{
	startmenu("", 0);
	runmenu();
	return 0;
}

void gameterminal(int x, int y)
{
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void startmenu(char* outputMessage, int chose)
{
	printf("\e[1;1H\e[2J"); //clear terminal
	gameterminal(0, 0);
	printheader();
	printmenu(outputMessage, chose);
}
 
void startgame(int chose)
{
	char player = 'X';
	char opponent = 'X';
	(chose == 1) ? (opponent = 'O') : (player = 'O');
	char message[PROGRAM_WIDTH] = "You chose ";
	strncat(message, &player, 1);
	strcat(message, ", your opponent takes ");
	strncat(message, &opponent, 1);
	strcat(message, ".");
	startmenu(message, chose);
}

void rungame(char* outputMessage, int chose)
{
	char player = 'X';
	char opponent = 'X';
	(chose == 1) ? (opponent = 'O') : (player = 'O');
	if (turn == 0)
	{
		srand(time(NULL));
		startingPlayer = rand() % 2;
		if (startingPlayer == 0) 
		{
			printgame("You are starting player!");
			for (int i = 0; i < 9; i++) game[i] = ' ';
			Sleep(1500);
			printgame("");
		}
		else
		{
			printgame("Opponent is starting player!");
			Sleep(1500);
			for (int i = 0; i < 9; i++) game[i] = ' ';
			goto OpponentsTurn;
		}
	}
	turn++;
	int input = 0;
	for (; scanf("%d", &input) != 1 || !(input == 1 || input == 2 || input == 3 ||
		input == 4 || input == 5 || input == 6 || input == 7 || input == 8 || input == 9) || isgraph(game[input - 1]);)
		//loop until input = 1 - 9
	{
		flushInput();
		isgraph(game[input - 1]) ?
		printgame("This spot is already taken!") :
		printgame("Enter a number from 1 - 9!");
	}
	game[input - 1] = player;
	printgame(outputMessage);
	if (turn > 5)checkVictory(player, opponent);
OpponentsTurn:
	Sleep(300);
	turn++;
	opponentsMove(player, opponent);
	printgame("");
	if (turn > 5) checkVictory(player, opponent);
	rungame(outputMessage, chose);
}

void runmenu(void)
{
	int input = 0;
	for (; (scanf("%d", &input) != 1) || !(input == 1 || input == 2 || input == 3);)
		//loop until input = 1 - 3
	{
		flushInput();
		startmenu("Enter a number from 1 - 3!", 0);
	}
	switch (input)
	{
	case 1:
	case 2:
	{
		startgame(input);
		Sleep(1500);
		rungame("", input);
		break;
	}
	case 3:
	{
		startmenu("Good Bye!", 3);
		exit(0);
	}
	}
}

void printheader(void)
{
	printf("-------------------------------------------------------------------\n");
	printf("                             TICTACTOE                             \n");
	printf("                        a game by Tom Runkel                       \n");
	printf("-------------------------------------------------------------------\n");
}
void printmenu(char* outputMessage, int chose)
{
	char choseOne[] = "  ";
	char choseTwo[] = "  ";
	char choseThree[] = "  ";
	switch (chose)
	{
	case 1: strcpy(choseOne, "->"); break;
	case 2: strcpy(choseTwo, "->"); break;
	case 3: strcpy(choseThree, "->"); break;
	}
	
	printf("                        %s 1 : Play with X                         \n", choseOne);
	printf("                        %s 2 : Play with O                         \n", choseTwo);
	printf("                        %s 3 : Exit                                \n", choseThree);
	printf("                                                                   \n");
	printf("%s\n", centerMessage(outputMessage));
	printf("                                                                   \n");
}
void printgame(char* outputMessage)
{
	printf("\e[1;1H\e[2J"); //clear terminal
	gameterminal(0, 0);
	printheader();
	printf("%s\n", centerMessage(outputMessage));
	printf("                                                                   \n");
	printf("                                                                   \n");
	printf("                              |       |                            \n");
	printf("                          %c   |   %c   |   %c                        \n", game[6], game[7], game[8]);
	printf("                              |       |                            \n");
	printf("                       -----------------------                     \n");
	printf("                              |       |                            \n");
	printf("                          %c   |   %c   |   %c                        \n", game[3], game[4], game[5]);
	printf("                              |       |                            \n");
	printf("                       -----------------------                     \n");
	printf("                              |       |                            \n");
	printf("                          %c   |   %c   |   %c                        \n", game[0], game[1], game[2]);
	printf("                              |       |                            \n");
	printf("                                                                   \n");
}

void printEndscreen(char* outputMessage)
{
	printf("-------------------------------------------------------------------\n");
	printf("%s\n", centerMessage(outputMessage));
	printf("                           won the game                            \n");
	printf("-------------------------------------------------------------------\n");
	printmenu("", 0);
	for (int i = 0; i < 9; i++) game[i] = ' ';
	turn = 0;
	runmenu();
}

char* centerMessage(char* outputMessage)
{
	strcpy(messageBuffer, "");
	for (int i = 0; i < ((PROGRAM_WIDTH - strlen(outputMessage)) / 2) - 1; i++)
	{
		strcat(messageBuffer, " ");
	}
	strcat(messageBuffer, outputMessage);
	return messageBuffer;
}

void flushInput(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {};
}

void checkVictory(char player, char opponent)
{
	for (int i = 0; i < 3; i++)
	{
		if (game[i] == player)
		{
			if (game[i] == game[i + 3] && game[i] == game[i + 6]) printEndscreen("YOU");
			if (i == 0 && game[i] == game[i + 4] && game[i] == game[i + 8]) printEndscreen("YOU");
			if (i == 2 && game[i] == game[i + 2] && game[i] == game[i + 4]) printEndscreen("YOU");
		} else if (game[3 * i] == player && game[3 * i] == game[3 * i + 1] && game[3 * i] == game[3 * i + 2]) printEndscreen("YOU");
		if (game[i] == opponent)
		{
			if (game[i] == game[i + 3] && game[i] == game[i + 6]) printEndscreen("OPPONENT");
			if (i == 0 && game[i] == game[i + 4] && game[i] == game[i + 8]) printEndscreen("OPPONENT");
			if (i == 2 && game[i] == game[i + 2] && game[i] == game[i + 4]) printEndscreen("OPPONENT");
		} else if (game[3 * i] == opponent && game[3 * i] == game[3 * i + 1] && game[3 * i] == game[3 * i + 2]) printEndscreen("OPPONENT");
	}
	if (turn > 8) printEndscreen("NOBODY");
}

void opponentsMove(char player, char opponent)
{
	if (checkPossibleVictory(opponent, opponent) == 1) return;
	if (checkPossibleVictory(player, opponent) == 1) return;
	srand(time(NULL));
	int randomValue = (rand() %9);
	for (; isgraph(game[randomValue]);) { randomValue = (rand() % 9); }
	game[randomValue] = opponent;
}

int checkPossibleVictory(char player, char opponent)
{
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!(((game[3 * i] + game[3 * i + 1] + game[3 * i + 2]) % (player * 2) - (j * 32))))
			{
				for (int k = 0; k < 3; k++)
				{
					if (isspace(game[3 * i + k]))
					{
						game[3 * i + k] = opponent;
						return 1;
					}
				}
			}
			if (!(((game[i] + game[i + 3] + game[i + 6]) % (player * 2) - (j * 32))))
			{
				for (int k = 0; k < 7; k += 3)
				{
					if (isspace(game[i + k]))
					{
						game[i + k] = opponent;
						return 1;
					}
				}
			}
			if (i == 0 && !(((game[i] + game[i + 4] + game[i + 8]) % (player * 2) - (j * 32))))
			{
				for (int k = 0; k < 9; k += 4)
				{
					if (isspace(game[i + k]))
					{
						game[i + k] = opponent;
						return 1;
					}
				}
			}
			if (i == 2 && !(((game[i] + game[i + 2] + game[i + 4]) % (player * 2) - (j * 32))))
			{
				for (int k = 0; k < 5; k += 2)
				{
					if (isspace(game[i + k]))
					{
						game[i + k] = opponent;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}