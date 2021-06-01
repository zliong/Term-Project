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
  
  
  }
