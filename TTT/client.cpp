// Code with modifications and inspiration from
// https://github.com/indradhanush/Multiplayer-tic-tac-toe/
// We used this as instructions to build our game, but not the protocol.
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "TTT.h"
#define  PORT "9987"

using namespace std;

//Function prototypes.

std::string getScoreboardThree(int socket);

int main(int argc, char* argv[]) {

	int sockfd, newsockfd, port_no, n, connectfd, bytes_sent, bytes_recvd;
	char cbuffer[512], sname[64], cname[64];
	char* ptr = &cbuffer[0];
	char* ptr_port = (char*)&PORT;
	struct sockaddr_in serv_addr;
	struct hostent* host;
	char message[MESSAGE_LENGTH];
	std::string messageBuilder;
	string server_ip;
	int port_input;
	string inputs, servName, first;
	int count = 0;
	int inp, x, y, ni, toss;
	int	inp_true = 0;
	char serv_choice, cli_choice, nc;
	char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;

	//Testing if correct arguments were used testing
	// if (argc != 2) {
	// 	perror("Incomplete arguments passed in.");
	// 	return 1;
	// }
	cout << "Please input the IP number for your host's game: ";
	cin >> server_ip;
	cout << "Please input the port number for your host game: ";
	cin >> port_no;
	//Checks to see if host exists
	host = gethostbyname(server_ip.c_str());
	if (host == NULL) {
		perror("Host doesn't exist!!");
		return 1;
	}
	//Attempts to create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("The socket couldn't be created.");
		return 1;
	}
	//Attempt to make connection to the hosts server
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr = *((struct in_addr*)host->h_addr);

	connectfd = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (connectfd == -1) //Closes if failed to connect
	{
		perror("Sorry. Could not connect to server.");
		return 1;
	}

	//If connected user prompted with name choice
	cout << "Enter your name : ";
	cin >> cname;

	cleanArray(message);
	read(sockfd, message, MESSAGE_LENGTH);
	if (!checkResponsePurpose(message, "USERNAMEQUERY")) {
		close(sockfd);
		cout << endl << "Server did not send correct response.";
		return 1;
	}
	servName = getMessageDetail(message);
	messageBuilder = "USERNAMEQUERY:";
	messageBuilder += cname;
	write(sockfd, messageBuilder.c_str(), messageBuilder.length());
	for (int i = 0; i < MESSAGE_LENGTH; i++) {
		message[i] = '\0';
	};
	cleanArray(message);
	read(sockfd, message, MESSAGE_LENGTH);
	//cout << "Message now contains:" << getMessageDetail(message) << endl;
	if (!checkResponsePurpose(message, "READYCHECK")) {
		close(sockfd);
		cout << endl << "Server did not send correct response.";
		return 1;
	}
	messageBuilder = "READYCHECK:";
	cout << endl << servName << " is ready to play tic tac toe, are you? [yes/no]: ";
	cin >> inputs;
	cout << endl << endl;
	messageBuilder += inputs;//might need to change to string
	//cout << "MessageBuilder = " << messageBuilder << ", Inputs = " << inputs << endl;
	//cout << "Message Builder c_str:" << messageBuilder.c_str() << endl;
	write(sockfd, messageBuilder.c_str(), messageBuilder.length());
	if (inputs == "no")
	{
		cout << "Client has exited the game!" << endl;
		return 0;
	}
	//cout << "Message Sent!!!!!!" << endl;
	//cout << "Creating game. Please wait..." << endl;
	sleep(2);
	cout << endl << "Game created!" << endl;
	cout << "You have joined " << servName << "'s game!" << endl;

	cleanArray(message);
	read(sockfd, message, MESSAGE_LENGTH);
	//check if message is WHOFIRST
	if (!checkResponsePurpose(message, "WHOFIRST")) {
		close(sockfd);
		std::cout << "The client did not receive the appropriate response. Closing.\n";
		return -1;
	}

	do
	{
		cout << "Please input a number between 1 and 10 (inclusive): ";
		cin >> inputs;
	} while (stoi(inputs) > 10 || stoi(inputs) < 1);
	messageBuilder = "WHOFIRST:";
	messageBuilder += inputs;
	write(sockfd, messageBuilder.c_str(), messageBuilder.length());	//send the input back to server
	for (int i = 0; i < MESSAGE_LENGTH; i++) {
		message[i] = '\0';
	};
	//get response from server
	cleanArray(message);
	read(sockfd, message, MESSAGE_LENGTH);
	first = getMessageDetail(message);
	//cout << "First contains " << first << endl;
	//cout << "Message is " << getMessageDetail(message) << endl; 
	for (int i = 0; i < MESSAGE_LENGTH; i++) {
		message[i] = '\0';
	};
	if (first == "server")
	{//If the server player wins they get to choose to be X or O
		cout << endl << servName << " Server goes first!" << endl;
		cout << servName << " is choosing. Please wait..." << endl << endl;
		//Client waits to receive server choice
		cleanArray(message);
		read(sockfd, message, MESSAGE_LENGTH);
		if (!checkResponsePurpose(message, "FIRSTCHOICE")) {
			close(sockfd);
			cout << endl << "Server did not send correct response.";
			return 1;
		}

		if (getMessageDetail(message) == "X") {
			serv_choice = 'X';
			cli_choice = 'O';
		}
		else {
			serv_choice = 'O';
			cli_choice = 'X';
		}
		cout << servName << " has chosen " << serv_choice << endl << endl << "You will play with " << cli_choice << endl;
		cout << endl << "Lets Play!" << endl << endl;

	}
	else
	{//If player wins the toss they get to go choose to be X or O
		cout << endl << "You pick first!" << endl;
		messageBuilder = "FIRSTCHOICE:";
		do
		{
			cout << cname << " Enter your Choice (X or O): ";
			cin >> cli_choice;
			if (cli_choice == 'X' || cli_choice == 'x')
			{
				serv_choice = 'O';
				cli_choice = 'X';
				//messageBuilder += "X";
				inp_true = 1;
				cout << endl << servName << " gets O." << endl << endl << "Lets Play!" << endl << endl;
			}
			else if (cli_choice == 'O' || cli_choice == 'o' || cli_choice == '0')
			{
				serv_choice = 'X';
				cli_choice = 'O';
				//messageBuilder += "O";
				inp_true = 1;
				cout << endl << servName << " gets X." << endl << endl << "Lets Play!" << endl << endl;
			}
			else
			{
				cout << "\nInvalid Choice! Please Choose Again..." << endl;
				inp_true == 0;
			}
		} while (inp_true == 0);
		//After valid first choice is made between X and O, hte info is sent to the server/client
		choice_buffer[0] = serv_choice;
		choice_buffer[1] = cli_choice;

		messageBuilder += cli_choice;
		write(sockfd, messageBuilder.c_str(), messageBuilder.length());

	}
	//Whichever player is X will get to go first
	if (serv_choice == 'X')
	{
		inp = 2;
		cout << servName << " will play first." << endl << endl;

	}
	else
	{
		inp = 1;
		cout << "You will play first." << endl << endl;
	}


	init();
	cout << endl << "Starting Game..." << endl;
	sleep(3);
	display();
	//Game officially starts. Loops till all moves are made, or player creates a line of three
	string xStr, yStr;

	while (count < 9)
	{
		messageBuilder = "TURNS:";

		if (inp % 2 == 0)
		{
			cout << endl << servName << "'s turn. Please wait..." << endl;
			cleanArray(message);
			read(sockfd, message, MESSAGE_LENGTH);
			if (!checkResponsePurpose(message, "TURNS")) {
				close(sockfd);
				cout << endl << "Server did not send correct response.";
				return 1;
			}
			xStr = getMessageDetail(message)[0];
			yStr = getMessageDetail(message)[2];
			x = std::stoi(xStr);
			y = std::stoi(yStr);
			ni = input(serv_choice, x, y);
			if (ni == 0)
			{
				inp++;
				cout << endl << servName << " has played. Updating Matrix..." << endl;
			}
		}
		else
		{ //Else portion has been changed
			cout << endl << "Your turn. Enter co-ordinates separated by a space : ";
			cin >> x >> y;
			ni = input(cli_choice, x, y);
			if (ni == 0)
			{
				inp++;
				messageBuilder += to_string(x) + " " + to_string(y); //convert input into string to send message

				cout << endl << "Updating Matrix..." << endl;

				write(sockfd, messageBuilder.c_str(), messageBuilder.length());
			}
		}

		count++;
		sleep(2);
		system("clear");
		display();
		//If player makes match then game ends and the winning player is congratulated
		if (count >= 5)
		{
			nc = check();
			if (nc == 'f')
				continue;
			else if (serv_choice == nc)
			{
				cout << endl << "You lose." << endl << servName << " has won." << endl;
				break;
			}
			else if (cli_choice == nc)
			{
				cout << endl << "Congrats! You have won!!!" << endl << servName << " lost." << endl;
				break;
			}
		}
		//If game isn't over it loops back to the top

	}
	//If there is a draw both players are notified
	if (nc == 'f')
		cout << endl << "Game ends in a draw." << endl;

	cout << endl << "Thank You for playing Tic-tac-Toe" << endl;
	cout << "Current top 3:\n";
	cout << getScoreboardThree(sockfd);
	close(sockfd);

	return 0;
}

std::string getScoreboardThree(int socket) {
	std::string scoreboardStorage;
	std::string messageBuilder;
	char message[MESSAGE_LENGTH];

	//Take the first 3 entries.
	for (int i = 0; i < 3; i++) {
		//Get message.
		cleanArray(message);
		read(socket, message, MESSAGE_LENGTH);

		//Check if message is valid.
		if (getMessagePurpose(message) != "SCOREENTRY") {
			std::cout << "The server did not send an expected response. Closing.\n";
			close(socket);
			return "";
		}

		//Add to the return value.
		scoreboardStorage += getMessageDetail(message);

		//tell server we're ready for more.
		//Reuse scoreboardstorage for efficiency.
		messageBuilder = "READY:";
		write(socket, messageBuilder.c_str(), messageBuilder.length());
	}
	return scoreboardStorage;
}
