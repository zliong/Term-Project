//This part was assigned to Logan Petersen to do, and I wrote.
// Code from 432 Program 1, with modifications from
// https://github.com/indradhanush/Multiplayer-tic-tac-toe/
// to improve the error handling and adapt the program to be
// a tic tac toe client and host at the same time.
#include <sys/types.h>    // socket, bind
#include <sys/socket.h>   // socket, bind, listen, inet_ntoa
#include <netinet/in.h>   // htonl, htons, inet_ntoa
#include    <unistd.h>    // read, write, close
#include   <strings.h>     // bzero
#include <netinet/tcp.h>  // SO_REUSEADDR
#include <sys/uio.h>      // writev
#include <sys/time.h>     //gettimeofday
#include <string>         //string
#include <iostream>
#include "TTT.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

//Function prototypes
void incrementScoreboard(std::string user);
std::string showScoreboard();
void sendTopThreeScoreboard(int socket);

int main(int argc, char* argv[]) {
    //Local Variables
    int port;
    std::string serverName;
    std::string clientName;
    std::string chat;
    std::string userInput;
    std::string messageBuilder;
    char message[MESSAGE_LENGTH];
    sockaddr_in acceptSock;
    sockaddr_in newsock;
    const int on = 1;
    int totalConnections = 1; //Tic tac toe is 2 players, this client is the host and player 1.

    //Improvement to make the program more user-friendly.
    // if (argc < 2) {
    //     std::cout << "Error occurred: bad port number argument.\n";
    //     return -1;
    // }
    // else {
    //     port = atoi(argv[1]);
    // }
    cout << "Please input the name of your server: ";
    cin >> serverName;
    cout << "Please input the port of your server: ";
    cin >> port;
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

    //Move on now, work on getting this game going!
    //Who are we playing?
    messageBuilder = "USERNAMEQUERY:";
    messageBuilder += serverName;
    write(newSd, messageBuilder.c_str(), messageBuilder.length());

    //Get response
    cleanArray(message);
    read(newSd, message, MESSAGE_LENGTH);
    if (!checkResponsePurpose(message, "USERNAMEQUERY")) {
        close(newSd);
        std::cout << "The client did not have an expected response. Closing.\n";
        return -1;
    }
    clientName = getMessageDetail(message);
    //cout << "Message contains: " << getMessageDetail(message) << endl;
    std::cout << "Your opponent is " << clientName << ". ";

    //Send message seeing if they're ready to play.
    messageBuilder = "READYCHECK:";
    write(newSd, messageBuilder.c_str(), messageBuilder.length());
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        message[i] = '\0';
    };
    //Get response
    do {
        cleanArray(message);
        read(newSd, message, MESSAGE_LENGTH);
        //cout << "Message Contains = " << getMessageDetail(message) << endl;
        //cout << "loop" << endl;
        if (!checkResponsePurpose(message, "READYCHECK")) {
            close(newSd);
            std::cout << "The client did not have an expected response. Closing.\n";
            return -1;
        }
        if (getMessageDetail(message) == "no")
        {
            cout << clientName << " has not accepted!" << endl;
            return -1;
        }
    } while (getMessageDetail(message) != "yes");

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
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        message[i] = '\0';
    };
    cleanArray(message);
    read(newSd, message, MESSAGE_LENGTH);
    if (!checkResponsePurpose(message, "WHOFIRST")) {
        close(newSd);
        std::cout << "The client did not have an expected response. Closing.\n";
        return -1;
    }

    //Compare number, say who's going first.
    string tempMsg;
    messageBuilder = "WHOFIRST:";
    //cout << "WHOMessage contains = " << getMessageDetail(message) << endl;
    if (stoi(userInput) > stoi(getMessageDetail(message))) {
        std::cout << "You go first.\n";
        messageBuilder += "server";
        tempMsg = "server";
    }
    else {
        std::cout << "Client goes first.\n";
        messageBuilder += "client";
        tempMsg = "client";
    }
    write(newSd, messageBuilder.c_str(), messageBuilder.length());

    //This is the beginning of the server version of the game code ---------------------------------
    //include these at the top
    std::string inputs, servName, first;
    int count = 0;
    int inp, x, y, ni, toss;
    int	inp_true = 0;
    char serv_choice, cli_choice, nc;
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        message[i] = '\0';
    };
    if (tempMsg == "client")
    {//If the server player loses, client get to choose to be X or O
        std::cout << std::endl << clientName << " goes first!" << std::endl;
        std::cout << clientName << " is choosing. Please wait..." << std::endl << std::endl;
        //Client waits to receive server choice
        cleanArray(message);
        read(newSd, message, MESSAGE_LENGTH);
        if (!checkResponsePurpose(message, "FIRSTCHOICE")) {
            close(newSd);
            std::cout << std::endl << "Server did not send correct response.";
            return 1;
        }

        if (getMessageDetail(message) == "O") {
            serv_choice = 'X';
            cli_choice = 'O';
        }
        else {
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
                //messageBuilder += "O";
                inp_true = 1;
                std::cout << std::endl << clientName << " gets X." << std::endl << std::endl << "Lets Play!" << std::endl << std::endl;
            }
            else if (serv_choice == 'X' || serv_choice == 'x' || serv_choice == 'X')
            {
                serv_choice = 'X';
                cli_choice = 'O';
                //messageBuilder += "O";
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
        messageBuilder += serv_choice;
        write(newSd, messageBuilder.c_str(), messageBuilder.length());

    }
    //Whichever player is X will get to go first
    if (cli_choice == 'X')
    {
        inp = 2;
        std::cout << clientName << " will play first." << std::endl << std::endl;

    }
    else
    {
        inp = 1;
        std::cout << "You will play first." << std::endl << std::endl;
    }


    init();
    std::cout << std::endl << "Starting Game..." << std::endl;
    sleep(3);
    display();
    //Game officially starts. Loops till all moves are made, or player creates a line of three
    std::string xStr, yStr;

    while (count < 9)
    {
        messageBuilder = "TURNS:";

        if (inp % 2 != 0)
        {
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
        else
        { //Else portion has been changed
            std::cout << std::endl << clientName << "'s turn. Please wait..." << std::endl;
            cleanArray(message);
            read(newSd, message, MESSAGE_LENGTH);
            if (!checkResponsePurpose(message, "TURNS")) {
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
            else if (cli_choice == nc)
            {
                incrementScoreboard(serverName);
                std::cout << std::endl << "You lose." << std::endl << clientName << " has won." << std::endl;
                break;
            }
            else if (serv_choice == nc)
            {
                incrementScoreboard(serverName);
                std::cout << std::endl << "Congrats! You have won!!!" << std::endl << clientName << " lost." << std::endl;
                break;
            }
        }
        //If game isn't over it loops back to the top

    }
    //If there is a draw both players are notified
    if (nc == 'f')
    {
        std::cout << std::endl << "Game ends in a draw." << std::endl;
        //send info to client that the game has drawn
    }

    std::cout << std::endl << "Thank You for playing Tic-tac-Toe" << std::endl;
    std::cout << "Current Scoreboard:\n" << showScoreboard();
    sendTopThreeScoreboard(newSd);
    close(newSd);

    return 0;
}

void incrementScoreboard(std::string user) {
    std::ofstream file;

    file.open("scoreboard.txt", std::ios::app); //Append mode.
    if (file.is_open()) {
        file << user;
        file << '\n';
        file.close();
    }
    else { std::cout << "scoreboard file failed to open.\n"; }

}
std::string showScoreboard() {

    std::ifstream file;
    vector<pair<int, std::string>> scoreboard;
    std::string entryGetter;
    bool userExists = false;

    file.open("scoreboard.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            userExists = false;
            std::getline(file, entryGetter);
            if (entryGetter != "") {
                for (pair<int, std::string> i : scoreboard) {
                    if (i.second == entryGetter) {
                        //User exists, increment.
                        userExists = true;
                        i.first++;
                        break; //Break since we found entry, we are done.
                    }
                }
                //If entry doesn't exist, add it.
                if (!userExists) {
                    scoreboard.push_back(make_pair(1, entryGetter));
                }
            }
        }
        file.close();
        //The reason the pair is int string is because it makes it easy to sort.
        sort(scoreboard.begin(), scoreboard.end());

        //Now we build the scoreboard as a string.
        //Reuse entrygetter since its purpose is done.
        entryGetter = "";
        for (pair<int, std::string> i : scoreboard) {
            entryGetter += i.second + ": " + to_string(i.first) + '\n';
        }
        return entryGetter;
    }
    else { return "scoreboard file failed to open.\n"; }
}

void sendTopThreeScoreboard(int socket) {
    string scoreboard = showScoreboard();
    std::string scoreboardStorage;
    char message[MESSAGE_LENGTH];

    //Take the first 3 entries.
    for (int i = 0; i < 3; i++) {
        //If the scoreboard isn't 3 big.
        scoreboardStorage = "";
        try {
            scoreboardStorage += scoreboard.substr(0, scoreboard.find('\n') + 1);
            scoreboard.erase(0, scoreboardStorage.length()); //To clear what we just substr.
        }
        catch (exception e) {
            scoreboardStorage = "";
        }
        scoreboardStorage = "SCOREENTRY:" + scoreboardStorage;

        write(socket, scoreboardStorage.c_str(), scoreboardStorage.length());

        //Wait for the client to say its ready for another piece.
        cleanArray(message);
        read(socket, message, MESSAGE_LENGTH);
        if (!checkResponsePurpose(message, "READY")) {
            close(socket);
            std::cout << "The client did not have an expected response. Closing.\n";
            return;
        }
    }
}