/* This is the client side code to connect to a tic-tac-toe game server.
 */
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "TTT.h"
#include "server.cpp
#define  PORT "9987"

using namespace std;

bool checkServerResponse(message message, std::string expected) {
    if(message.purpose != expected) {
        return false;
    }
    return true;
}

int main(int argc, char* argv[]){

	int sockfd, newsockfd, port_no, n, connectfd, bytes_sent, bytes_recvd;
	char cbuffer[512], sname[64], cname[64];
	char* ptr = &cbuffer[0];
	char* ptr_port = (char*)&PORT;
	struct sockaddr_in serv_addr;
	struct hostent* host;
 
        string inputs, servName, first;
	int count = 0;
	int inp, x, y, ni, toss;
	int	inp_true = 0;
	char serv_choice, cli_choice, nc;
	char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;

	system("clear");

	//Testing if correct arguments were used testing
	if (argc != 2) {
	     	perror("Incomplete arguments passed in.");
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
      

        
        message messageHandler;
        read(sockfd, &messageHandler, 100);
        if(!checkServerResponse(messageHandler, "USERNAMEQUERY")){
            close(sockfd);
            cout << endl << "Server did not send correct response.":
            return 1;
        }
        messageHandler.purpose = "USERNAMEQUERY";
        servName = messageHandler.details;
        messageHandler.details = cname;//might need to change to string
        write(sockfd, &messageHandler, sizeof(messageHandler));

        read(sockfd, &messageHandler, 100);
        if(!checkServerResponse(messageHandler, "READYCHECK")){
            close(sockfd);
            cout << endl << "Server did not send correct response.":
            return 1;
        }
        messageHandler.purpose = "READYCHECK";
        cout << endl << servName << " is ready to play tic tac toe, are you? [yes/no]: "
        cin >> inputs;
        cout << endl << endl;
        messageHandler.details = inputs;//might need to change to string
        write(sockfd, &messageHandler, sizeof(messageHandler));

        cout << "Creating game. Please wait..." << endl;
        sleep(2);
        cout << endl << "Game created!" << endl;
        cout << "You have joined " << servName << "'s game!" << endl;





	//whoisfirst goes here
        
	
	

	
	if (first == "server")
	{//If the server player wins they get to choose to be X or O
		cout << endl << sname << " Server goes first!" << endl;
		cout << servName << " is choosing. Please wait..." << endl << endl;
		//Client waits to receive server choice
		read(sockfd, &messageHandler, 100);
                if(!checkServerResponse(messageHandler, "FIRSTCHOICE")){
                close(sockfd);
                cout << endl << "Server did not send correct response.":
                return 1;
                }

		if(messageHandler.details == "X"){
	        serv_choice = 'X';
		cli_choice = 'O';
                } else {
                serv_choice = 'O';
		cli_choice = 'X';
                }
		cout << sname << " has chosen " << serv_choice << endl << endl << "You will play with " << cli_choice << endl;
		cout << endl << "Lets Play!" << endl << endl;
		
	}
	else
	{//If player wins the toss they get to go choose to be X or O
		cout << endl << "You pick first!" << endl;
		do
		{
			cout << cname << " Enter your Choice (X or O): ";
			cin >> cli_choice;
			if (cli_choice == 'X' || cli_choice == 'x')
			{
				serv_choice = 'O';
				cli_choice = 'X';
                                messageHandler.details = "X"
				inp_true = 1;
				cout << endl << servName << " gets O." << endl << endl << "Lets Play!" << endl << endl;
			}
			else if (cli_choice == 'O' || cli_choice == 'o' || cli_choice == '0')
			{
				serv_choice = 'X';
				cli_choice = 'O';
                                messageHandler.details = "O";
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
                
		messageHandler.purpose = "FIRSTCHOICE";
                write(sockfd, &messageHandler, sizeof(messageHandler));

	}
	//Whichever player is X will get to go first
	if (serv_choice == 'X')
	{
		inp = 1;
		cout << sname << " will play first." << endl << endl;

	}
	else
	{
		inp = 2;
		cout << "You will play first." << endl << endl;
	}


	init();
	cout << endl << "Starting Game..." << endl;
	sleep(3);
	display();
	//Game officially starts. Loops till all moves are made, or player creates a line of three
        string xStr, yStr;
        messageHandler.purpose = "TURNS";
	while (count < 9)
	{
		memset(&co_ordinates_buffer, 0, sizeof(co_ordinates_buffer));

		if (inp % 2 != 0)
		{
			cout << endl << servName << "'s turn. Please wait..." << endl;
			read(sockfd, &messageHandler, 100);
			if(!checkServerResponse(messageHandler, "TURNS")){
                        close(sockfd);
                        cout << endl << "Server did not send correct response.":
                        return 1;
                        }
                        xStr = messageHandler.details[0];
                        yStr = messageHandler.details[2];
			x = std::stoi(xStr);
			y = std::stoi(yStr);
			ni = input(serv_choice, x, y);
			if (ni == 0)
			{
				inp++;
				cout << endl << sname << " has played. Updating Matrix..." << endl;
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
				messageHandler.details = to_string(x) + " " + to_string(y); //convert input into string to send message
        
				cout << endl << "Updating Matrix..." << endl;

				write(sockfd, &messageHandler, sizeof(messageHandler));
			}
		}

		count++;
		sleep(2);
		system("clear");
		display();
		//If player makes match then game ends and the winning player is congradulated
		if (count >= 5)
		{
			nc = check();
			if (nc == 'f')
				continue;
			else if (serv_choice == nc)
			{
				cout << endl << "You loose." << endl << sname << " has won." << endl;
				break;
			}
			else if (cli_choice == nc)
			{
				cout << endl << "Congrats! You have won!!!" << endl << sname << " lost." << endl;
				break;
			}
		}
		//If game isn't over it loops back to the top

	}
	//If there is a draw both players are notified
	if (nc == 'f')
		cout << endl << "Game ends in a draw." << endl;


	cout << endl << "Thank You for playing Tic-tac-Toe" << endl;
	close(sockfd);

	return 0;
}

