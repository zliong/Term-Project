/* This is the client side code to connect to a tic-tac-toe game server.
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "tictac.h"
#define  PORT "9987"

using namespace std;

int main(int argc, char* argv[]) {

	int sockfd, newsockfd, port_no, n, connectfd, bytes_sent, bytes_recvd;
	char cbuffer[512], sname[64], cname[64];
	char* ptr = &cbuffer[0];
	char* ptr_port = (char*)&PORT;
	struct sockaddr_in serv_addr;
	struct hostent* host;

	int count = 0;
	int inp, x, y, ni, toss;
	int	inp_true = 0;
	char serv_choice, cli_choice, nc;
	char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;

	system("clear");

	//Testing if correct arguments were used testing
	if (argc != 2) {
	     	perror("Incomplete arguments passed in.")
			return 1;
	}
	//Checks to see if host exists
	port_no = atoi(ptr_port);
	host = gethostbyname(argv[1]);
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
	do//This is where the JOIN part of the requirement will be implimented
	{
		//Checks to see if player data can be sent
		static int flag = 0;
		bytes_sent = send(sockfd, &cname, sizeof(cname), 0);
		if (bytes_sent == -1 && flag == 0)
		{
			cout << "PLAYER DATA NOT SENT!" << endl << "Trying Again...";
			continue;
		}
		else
		{//If player data can be sent then client waits to receive name of server play back 
			//to telll user they have connected & who they are gonna play with
			cli_choice = 'X';

			flag = 1;
			memset(&sname, 0, sizeof(sname));
			bytes_recvd = recv(sockfd, &sname, sizeof(sname), 0);
			if (bytes_recvd == -1)
				cout << "COULD NOT ACQUIRE PLAYER INFORMATION!" << endl << "Trying Again..." << endl;
			else
				cout << "You have joined " << sname << " for a game of Tic-Tac-Toe." << endl;
		}
	} while (bytes_sent == -1 || bytes_recvd == -1);

	cout << "Creating game. Please wait..." << endl;
	sleep(2);
	cout << endl << "Game created!" << endl << endl << "Doing a toss...";
	//Waits to receive toss information which decides who goes first
	bytes_recvd = recv(sockfd, &toss_buffer, sizeof(toss_buffer), 0);
	if (bytes_recvd == -1)
	{
		perror("TOSS BUFFER not received");
		return 1;
	}

	toss = toss_buffer - '0';
	if (toss == 0)
	{//If the server player wins they get to choose to be X or O
		cout << endl << sname << " WON the toss." << endl;
		cout << sname << " is choosing. Please wait..." << endl << endl;
		//Client waits to receive server choice
		memset(&choice_buffer, 0, sizeof(choice_buffer));
		bytes_recvd = recv(sockfd, &choice_buffer, sizeof(choice_buffer), 0);
		if (bytes_recvd == -1)
		{
			perror("CHOICE BUFFER not received!");
			return 1;
		}
		else
		{
			serv_choice = choice_buffer[0];
			cli_choice = choice_buffer[1];
			cout << sname << " has chosen " << serv_choice << endl << endl << "You will play with " << cli_choice << endl;
			cout << endl << "Lets Play!" << endl << endl;
		}
	}
	else
	{//If player wins the toss they get to go choose to be X or O
		cout << endl << "You won the TOSS!" << endl;
		do
		{
			cout << cname << " Enter your Choice (X or O): ";
			cin >> cli_choice;
			if (cli_choice == 'X' || cli_choice == 'x')
			{
				serv_choice = 'O';
				cli_choice = 'X';
				inp_true = 1;
				cout << endl << sname << " gets O." << endl << endl << "Lets Play!" << endl << endl;
			}
			else if (cli_choice == 'O' || cli_choice == 'o' || cli_choice == '0')
			{
				serv_choice = 'X';
				cli_choice = 'O';
				inp_true = 1;
				cout << endl << sname << " gets X." << endl << endl << "Lets Play!" << endl << endl;
			}
			else
			{
				cout << "\nInvalid Choice! Please Choose Again..." << endl;
				inp_true == 0;
			}
		} while (inp_true == 0);
		//After valid first choice is made between X and O, hte info is sent to the server/client
		memset(&choice_buffer, 0, sizeof(choice_buffer));
		choice_buffer[0] = serv_choice;
		choice_buffer[1] = cli_choice;

		bytes_sent = send(sockfd, &choice_buffer, sizeof(choice_buffer), 0);
		if (bytes_sent == -1)
		{
			perror("CHOICE BUFFER could not be sent.");
			return 1;
		}

	}
  
  }
