# Term-Project

How to play the game (TTT Folder only)

1.Compile the two cpp's files: Server and Client. (Hint: g++ server.cpp -o server and g++ client.cpp -o client)

2. First Run the Server (./server)


3. It will ask you for a server name (first name or any name) and port number as inputs (Ex: 8080, 9987). Once you finish those inputs, it will display that it is waiting on client


4. Then Run the client (./client). It asks for the ip address (default input is 127.0.0.1) and port of the server (depends on what you inputted). Then add your name


5. If connected successfully, server will say that a client has connect. If you (the client), want to exit, just say "no" when you are prompted. If you want to play the game,just
  say "yes."
  
  
6. Finally, it will ask you to pick a number between 0 to 10, whoever gets the highest will go pick their variable they want (X or O). 

7. You can play the standard game of Tic-Tac-Toe (Hint: You can use the on-screen guidance to input commands). Scoreboard will be updated at the end of the game.

Note: The only way to truly Log off is the prompt to play the game or not. If you exit the game in the middle of it, then the server or client must down his or her own application. Therefore, there is no logging off mid-game. Only at the beginning or ending of the application.

Note: X will always go first in placing markers.
