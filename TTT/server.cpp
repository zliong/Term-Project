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

int main(int argc, char* argv[]) {
    //Local Variables
    int port;

    //Improvement to make the program more user-friendly.
    try {
        port = atoi(argv[1]);
    }
    catch (std::exception e) {
        std::cout << "Error occurred: bad port number argument.\n";
        return -1;
    }
    sockaddr_in acceptSock;
    const int on = 1;
    sockaddr_in newsock;
    int totalConnections = 1; //Tic tac toe is 2 players, this client is the host and player 1.

    bzero((char*) &acceptSock, sizeof(acceptSock));  // zero out the data structure
    acceptSock.sin_family = AF_INET;   // using IP
    acceptSock.sin_addr.s_addr = htonl(INADDR_ANY); // listen on any address this computer has
    acceptSock.sin_port = htons(port);  // set the port to listen on

    int serverSd = socket(AF_INET, SOCK_STREAM, 0); // creates a new socket for IP using TCP

    setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(int));  // this lets us reuse the socket without waiting for hte OS to recycle it

    // Bind the socket
    bind(serverSd, (sockaddr*) &acceptSock, sizeof(acceptSock));  // bind the socket using the parameters we set earlier

    // Listen on the socket
    listen(serverSd, totalConnections);  // listen on the socket and allow up to n connections to wait.
    socklen_t newsockSize = sizeof(newsock);

    std::cout << "Beginning to watch for another player!\n";

    int newSd = accept(serverSd, (sockaddr *)&newsock, &newsockSize);
    std::cout << "Got a client to play tic tac toe with!";

    return 0;
}