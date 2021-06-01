#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

char Board[3][3], player1, player2;
void init();
void display();
int input(char input, int row, int col);
char check();

//initialize the Tic-Tac-Toe Board (-)
void init()
{
	int row, col;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			Board[row][col] = '-';
		}
	}
}

void display()
{
	int row, col;
	system("clear"); //clear the console
	cout << "Welcome to Tic-Tac-Toe Game!\n\nThe Grid Coordinates (x,y):" << endl << endl;
	cout << "Here's the possible commands that user can input (Don't have comma in between x and y): " << endl;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			cout << "[x,y] = " << row << " " << col << "     " << endl;
		}
	}
	cout << " \nThe Current Tic-Tac-Toe Board: " << endl;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			cout << "||" << Board[row][col] << "||";
		}
		cout << endl;
	}
}

int input(char input, int row, int col)
{
	if (row >= 0 && row <= 2 && col >= 0 && col <= 2)
		if (Board[row][col] == '-')
		{
			Board[row][col] = input;
			return 0;
		}
		else
		{
			cout << endl << "Co-ordinates " << row << " " << col << " are already used!" << endl << "Press ENTER to continue..." << endl;
			do {} 			while (cin.get() != '\n');
			return 1;
		}
	else
	{
		cout << "\nInvalid Co-ordinates!" << endl << "Press ENTER to continue..." << endl;
		do {} 		while (cin.get() != '\n');
		return 1;
	}
}

char check()
{
	int row, col;
	char temp;

	for (col = 0; col < 3; col++)
	{
		if (Board[0][col] != '-')
		{
			temp = Board[0][col];
			if (col == 0)
			{
				if (Board[0][1] == temp)
					if (Board[0][2] == temp)
						return temp;
				if (Board[1][0] == temp)
					if (Board[2][0] == temp)
						return temp;
				if (Board[1][1] == temp)
					if (Board[2][2] == temp)
						return temp;
			}
			else if (col == 1)
			{
				if (Board[1][1] == temp)
					if (Board[2][1] == temp)
						return temp;
			}
			else
			{
				if (Board[1][2] == temp)
					if (Board[2][2] == temp)
						return temp;
			}
		}
	}
	for (row = 1; row < 3; row++)
	{
		if (Board[row][0] != '-')
		{
			temp = Board[row][0];
			if (row == 1)
			{
				if (Board[1][1] == temp)
					if (Board[1][2] == temp)
						return temp;
			}
			else
			{
				if (Board[2][1] == temp)
					if (Board[2][2] == temp)
						return temp;
				if (Board[1][1] == temp)
					if (Board[0][2] == temp)
						return temp;
			}
		}
	}

	return 'f';

}
