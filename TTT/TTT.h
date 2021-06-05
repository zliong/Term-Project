#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

const int MESSAGE_LENGTH = 100;

char Board[3][3], player1, player2;
void init();
void display();
int input(char input, int row, int col);
char check();

//Clean the char message to ensure good input.
void cleanArray(char* message) {
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        message[i] = '\0';
    };
}

//See if the response purpose matches what we expect.
bool checkResponsePurpose(char message[MESSAGE_LENGTH], std::string expected) {
	string strMessage(message);
	strMessage = strMessage.substr(0, strMessage.find(':'));
	//cout << "strMessage contains " << strMessage << endl;
	if (strMessage == expected) { return true; }
	return false;
}

//Get a message's purpose.
std::string getMessagePurpose(char message[MESSAGE_LENGTH]) {
	string strMessage(message);
	return strMessage.substr(0, strMessage.find(':'));
}

//get a message's details.
std::string getMessageDetail(char message[MESSAGE_LENGTH]) {
	string strMessage(message);
	//cout << "strMessage = " << strMessage << endl;
	return strMessage.substr(strMessage.find(':') + 1, strMessage.length());
}
string convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

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
			do {} while (cin.get() != '\n');
			return 1;
		}
	else
	{
		cout << "\nInvalid Co-ordinates!" << endl << "Press ENTER to continue..." << endl;
		do {} while (cin.get() != '\n');
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