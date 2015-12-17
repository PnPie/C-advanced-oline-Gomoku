# Advanced online Gomoku
An online advanced Gomoku game in terminal written in C

Gomoku is an abstract strategy board game, also called Five in a Row. Here our advanced Gomoku allows to have more than 2 players, and we can also define the size of board and even the number of stones required to win.

## Compile
Using Makefile to compile and generate two executable programs **server** and **client** in folder *bin*:  
`make`

## Execute
Launch server program with an indicated port, ex:  
`./server 8000`

Launch client program with the address and the port of server (ex. to connect to it locally):  
`./client 127.0.0.1 8000`

Then you will be asked for a password to identify whether you are an **administrator** or a **client player**.

- administrator: define the rule of this match, including *number of clients*, *size of board*, *number of stones in a row to win*.
- client: connect to the server to play.

We always need an administrator to connect to the server firstly to define the game, and then the clients can participate in.
