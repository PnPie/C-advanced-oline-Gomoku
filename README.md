# Advanced online Gomoku
An online advanced Gomoku game in terminal written in C

Gomoku is an abstract strategy board game, also called Five in a Row. Here our advanced Gomoku allows to have more than 2 players, and we can also define the size of board and even the number of stones required to win.

## To use
Use Makefile to compile and generate two programs, the **serveur** and the **client**  
    make
-launch the serveur program with a indicated port
    ./serveur 8000
-launch the client program with address and port of the server, here for example we connect to it locally
    ./client 127.0.0.1 8000
Then it will ask you for a password to identify whether you are an administrator or a client player.
The administrator can define the rule of this match, including the number of client, the size of board, and the number of stones in a row to win.
Then the client can connect to the server to play.
