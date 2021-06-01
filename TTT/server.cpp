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

//Message Structure
struct message {
    std::string purpose;
    std::string details;
};

//Function Prototype
void* ConnectionProcessor(void* socket);
bool checkClientResponse(message message, std::string expected);

int main(int argc, char* argv[]) {
    //Local Variables
    int port;
    std::string serverName = "The original, epic, truly amazing... Server 1.";
    std::string clientName;
    std::string chat;
    std::string input;
    message messageHandler;

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
    std::cout << "Got a client to play tic tac toe with!";

    //Create new thread for chat message getting.
    //pthread_t thread;

    //Send the input to a thread to deal with it.
    //pthread_create(&thread, nullptr, ConnectionProcessor, (void*) newSd);

    //Move on now, work on getting this game going!
    //Who are we playing?
    messageHandler.purpose = "USERNAMEQUERY ";
    messageHandler.details = serverName;
    write(newSd, &messageHandler, sizeof(messageHandler));

    //Get response
    read(newSd, &messageHandler, 100); //TODO: set to a good number.
    if (!checkClientResponse(messageHandler, "USERNAMEQUERY")) {
        close(newSd);
        std::cout << "The client did not have an expected response. Closing.\n";
        return -1;
    }
    clientName = messageHandler.purpose;

    //Send message seeing if they're ready to play.
    messageHandler.purpose = "READYCHECK";
    messageHandler.details = nullptr;
    write(newSd, &messageHandler, sizeof(messageHandler));

    //Get response
    while(messageHandler.details != "yes") {
        read(newSd, &messageHandler, 100); //TODO: set to a good number.
        if (!checkClientResponse(messageHandler, "READYCHECK")) {
            close(newSd);
            std::cout << "The client did not have an expected response. Closing.\n";
            return -1;
        }
    }

    //We're ready to play, choose a side.
    messageHandler.purpose = "WHOFIRST";
    messageHandler.details = "1 10";
    write(newSd, &messageHandler, sizeof(messageHandler));

    //Get from us our number.
    std::cout << "Pick a number between 1 and 10 (inclusive).\n";
    std::cin >> input;

    //Get client response.
    read(newSd, &messageHandler, 100); //TODO: set to a good number.
    if (!checkClientResponse(messageHandler, "WHOFIRST")) {
        close(newSd);
        std::cout << "The client did not have an expected response. Closing.\n";
        return -1;
    }
    
    //Compare number, say who's going first.
    messageHandler.purpose = "WHOFIRST";
    if(input > messageHandler.details) {
        std::cout << "You go first.\n";
        messageHandler.details = "server";
    }
    else {
        std::cout << "Client goes first.\n";
        messageHandler.details = "client";
    }
    write(newSd, &messageHandler, sizeof(messageHandler));

}

//Check client response, if client response is bad (unexpected) the program reports and ends.
bool checkClientResponse(message message, std::string expected) {
    if(message.purpose != expected) {
        return false;
    }
    return true;
}

void* ConnectionProcessor(void* socket) {
    return 0;
}