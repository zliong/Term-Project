//This part was assigned to Logan Petersen to do.
// Code from 432 Program 1, with net code modifications from
// https://github.com/indradhanush/Multiplayer-tic-tac-toe/
// to improve the error handling and adapt the program to be
// a tic tac toe client and host at the same time.
#include <sys/types.h>    // socket, bind
#include <sys/socket.h>   // socket, bind, listen, inet_ntoa
#include <netinet/in.h>   // htonl, htons, inet_ntoa
#include <arpa/inet.h>    // inet_ntoa
#include    <unistd.h>    // read, write, close
#include   <strings.h>     // bzero
#include <netinet/tcp.h>  // SO_REUSEADDR
#include <sys/uio.h>      // writev
#include <sys/time.h>     //gettimeofday
#include <string>         //string
#include <pthread.h>      //Pthread
#include <iostream>
#include "TTT.h"

//Function Prototype
void* ConnectionProcessor(void* socket);

int main(int argc, char* argv[]) {
    //Local Variables
    int port;
    std::string serverName = "The original, epic, truly amazing... Server 1.";
    std::string clientName;
    std::string chat;
    std::string userInput;
    std::string messageBuilder;
    char message[MESSAGE_LENGTH];

    //Improvement to make the program more user-friendly.
    if(argc < 2) {
        std::cout << "Error occurred: bad port number argument.\n";
        return -1;
    }
    else {
        port = atoi(argv[1]);
    }

    sockaddr_in acceptSock;
    const int on = 1;
    sockaddr_in newsock;
    int totalConnections = 1; //Tic tac toe is 2 players, this client is the host and player 1.

    bzero((char*)&acceptSock, sizeof(acceptSock));  // zero out the data structure
    acceptSock.sin_family = AF_INET;   // using IP
    acceptSock.sin_addr.s_addr = htonl(INADDR_ANY); // listen on any address this computer has
    acceptSock.sin_port = htons(port);  // set the port to listen on

    int serverSd = socket(AF_INET, SOCK_STREAM, 0); // creates a new socket for IP using TCP

    setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(int));  // this lets us reuse the socket without waiting for hte OS to recycle it

    // Bind the socket
    bind(serverSd, (sockaddr*)&acceptSock, sizeof(acceptSock));  // bind the socket using the parameters we set earlier

    // Listen on the socket
    listen(serverSd, totalConnections);  // listen on the socket and allow up to n connections to wait.
    socklen_t newsockSize = sizeof(newsock);

    std::cout << "Beginning to watch for another player!\n";

    int newSd = accept(serverSd, (sockaddr*)&newsock, &newsockSize);
    std::cout << "Got a client to play tic tac toe with!\n";

    //Create new thread for chat message getting.
    //pthread_t thread;

    //Send the input to a thread to deal with it.
    //pthread_create(&thread, nullptr, ConnectionProcessor, (void*) newSd);

    //Move on now, work on getting this game going!
    //Who are we playing?
    messageBuilder = "USERNAMEQUERY:";
    messageBuilder += serverName;
    write(newSd, messageBuilder.c_str(), messageBuilder.length());

    //Get response
    cleanMessage(message);
    read(newSd, message, MESSAGE_LENGTH);
    if (!checkResponsePurpose(message, "USERNAMEQUERY")) {
        close(newSd);
        std::cout << "The client did not have an expected response. Closing.\n";
        return -1;
    }
    clientName = getMessageDetail(message);
    std::cout << "Your opponent is " << clientName << '.';

    //Send message seeing if they're ready to play.
    messageBuilder = "READYCHECK:";
    write(newSd, messageBuilder.c_str(), messageBuilder.length());

    //Get response
    while(getMessageDetail(message) != "yes") {
        cleanMessage(message);
        read(newSd, message, MESSAGE_LENGTH);
        if (!checkResponsePurpose(message, "READYCHECK")) {
            close(newSd);
            std::cout << "The client did not have an expected response. Closing.\n";
            return -1;
        }
    }
/*
    //We're ready to play, choose a side.
    messageHandler.purpose = "WHOFIRST";
    messageHandler.details = "1 10";
    write(newSd, &messageHandler, sizeof(messageHandler));

    //Get from us our number.
    std::cout << "Pick a number between 1 and 10 (inclusive).\n";
    std::cin >> userInput;

    //Get client response.
    read(newSd, message, MESSAGE_LENGTH);
    if (!checkClientResponse(messageHandler, "WHOFIRST")) {
        close(newSd);
        std::cout << "The client did not have an expected response. Closing.\n";
        return -1;
    }
    
    //Compare number, say who's going first.
    messageHandler.purpose = "WHOFIRST";
    if(stoi(userInput) > stoi(messageHandler.details)) {
        std::cout << "You go first.\n";
        messageHandler.details = "server";
    }
    else {
        std::cout << "Client goes first.\n";
        messageHandler.details = "client";
    }
    write(newSd, &messageHandler, sizeof(messageHandler));
*/
    //Done by Carson Riland.
    //My new code for the game with edits and notes for server implementation
    //We're ready to play, choose a side.
    messageBuilder = "WHOFIRST:";
    messageBuilder += "1 10";
    write(newSd, messageBuilder.c_str(), messageBuilder.length());

    //Get from us our number.
    std::cout << "Pick a number between 1 and 10 (inclusive).\n";
    std::cin >> userInput;

    //Get client response.
    cleanMessage(message);
    read(newSd, message, MESSAGE_LENGTH);
    if (!checkResponsePurpose(message, "WHOFIRST")) {
        close(newSd);
        std::cout << "The client did not have an expected response. Closing.\n";
        return -1;
    }
    
    //Compare number, say who's going first.
    messageBuilder = "WHOFIRST:";
    if(stoi(userInput) > stoi(getMessageDetail(message))) {
        std::cout << "You go first.\n";
        messageBuilder += "server";
    }
    else {
        std::cout << "Client goes first.\n";
        messageBuilder += "client";
    }
    write(newSd, messageBuilder.c_str(), messageBuilder.length());

    //This is the beginning of the server version of the game code ---------------------------------
    //include these at the top
    std::string inputs, servName, first;
	int count = 0;
	int inp, x, y, ni, toss;
	int	inp_true = 0;
	char serv_choice, cli_choice, nc;

    if (getMessageDetail(message) == "client")
	{//If the server player wins they get to choose to be X or O
		std::cout << std::endl << serverName << " Server goes first!" << std::endl;
        std::cout << clientName << " is choosing. Please wait..." << std::endl << std::endl;
		//Client waits to receive server choice
        cleanMessage(message);
        read(newSd, message, MESSAGE_LENGTH);
                if(!checkResponsePurpose(message, "FIRSTCHOICE")){
                close(newSd);
                    std::cout << std::endl << "Client did not send correct response.";
                return 1;
                }

		if(getMessageDetail(message) == "O"){
	        serv_choice = 'X';
		cli_choice = 'O';
                } else {
                serv_choice = 'O';
		cli_choice = 'X';
                }
        std::cout << clientName << " has chosen " << cli_choice << std::endl << std::endl << "You will play with " << serv_choice << std::endl;
        std::cout << std::endl << "Lets Play!" << std::endl << std::endl;
		
	}
	else
	{//If player wins the toss they get to go choose to be X or O
        messageBuilder = "FIRSTCHOICE:";
        std::cout << std::endl << "You pick first!" << std::endl;
		do
		{
            std::cout << serverName << " Enter your Choice (X or O): ";
            std::cin >> serv_choice;
			if (serv_choice == 'O' || serv_choice == 'o')
			{
				serv_choice = 'O';
				cli_choice = 'X';
                messageBuilder += "O";
				inp_true = 1;
                std::cout << std::endl << clientName << " gets X." << std::endl << std::endl << "Lets Play!" << std::endl << std::endl;
			}
			else if (serv_choice == 'X' || serv_choice == 'x' || serv_choice == 'X')
			{
				serv_choice = 'X';
				cli_choice = 'O';
                messageBuilder += "O";
				inp_true = 1;
                std::cout << std::endl << clientName << " gets O." << std::endl << std::endl << "Lets Play!" << std::endl << std::endl;
			}
			else
			{
                std::cout << "\nInvalid Choice! Please Choose Again..." << std::endl;
				inp_true == 0;
			}
		} while (inp_true == 0);
		//After valid first choice is made between X and O, hte info is sent to the server/client

        write(newSd, messageBuilder.c_str(), messageBuilder.length());

	}
	//Whichever player is X will get to go first
	if (cli_choice == 'X')
	{
		inp = 1;
        std::cout << serverName << " will play first." << std::endl << std::endl;

	}
	else
	{
		inp = 2;
        std::cout << "You will play first." << std::endl << std::endl;
	}


	init();
    std::cout << std::endl << "Starting Game..." << std::endl;
	sleep(3);
	display();
	//Game officially starts. Loops till all moves are made, or player creates a line of three
    std::string xStr, yStr;
        messageBuilder = "TURNS:";
	while (count < 9)
	{
		

		if (inp % 2 != 0)
		{
            std::cout << std::endl << clientName << "'s turn. Please wait..." << std::endl;
            cleanMessage(message);
            read(newSd, message, MESSAGE_LENGTH);
			if(!checkResponsePurpose(message, "TURNS")){
                close(newSd);
                std::cout << std::endl << "Server did not send correct response.";
                return 1;
            }
            xStr = getMessageDetail(message)[0];
            yStr = getMessageDetail(message)[2];
			x = std::stoi(xStr);
			y = std::stoi(yStr);
			ni = input(cli_choice, x, y);
			if (ni == 0)
			{
				inp++;
                std::cout << std::endl << serverName << " has played. Updating Matrix..." << std::endl;
			}
		}
		else
		{ //Else portion has been changed
            std::cout << std::endl << "Your turn. Enter co-ordinates separated by a space : ";
            std::cin >> x >> y;
			ni = input(serv_choice, x, y);
			if (ni == 0)
			{
				inp++;
				messageBuilder += std::to_string(x) + " " + std::to_string(y); //convert input into string to send message

                std::cout << std::endl << "Updating Matrix..." << std::endl;

                write(newSd, messageBuilder.c_str(), messageBuilder.length());
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
                std::cout << std::endl << "You loose." << std::endl << serverName << " has won." << std::endl;
				break;
			}
			else if (cli_choice == nc)
			{
                std::cout << std::endl << "Congrats! You have won!!!" << std::endl << serverName << " lost." << std::endl;
				break;
			}
		}
		//If game isn't over it loops back to the top

	}
	//If there is a draw both players are notified
	if (nc == 'f')
        std::cout << std::endl << "Game ends in a draw." << std::endl;


    std::cout << std::endl << "Thank You for playing Tic-tac-Toe" << std::endl;
	close(newSd);

	return 0;
}

void* ConnectionProcessor(void* socket) {
    return 0;
}
