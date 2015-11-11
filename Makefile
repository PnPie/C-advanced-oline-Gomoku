CC = gcc -Wall
CLIENT_DIR = ./src/client
SERVER_DIR = ./src/server
BIN_DIR = ./bin

SERVER_EXE = ${BIN_DIR}/server
CLIENT_EXE = ${BIN_DIR}/client

all: $(SERVER_EXE) $(CLIENT_EXE)

$(SERVER_EXE): ${SERVER_DIR}/server_socket.o ${SERVER_DIR}/server_lib.o
	$(CC) -o $@ $^

$(CLIENT_EXE): ${CLIENT_DIR}/client_socket.o ${CLIENT_DIR}/client_lib.o
	$(CC) -o $@ $^

server_socket.o: ${SERVER_DIR}/server_socket.c ${SERVER_DIR}/server_lib.h
	$(CC) -c $<

server_lib.o: ${SERVER_DIR}/server_lib.c ${SERVER_DIR}/server_lib.h
	$(CC) -c $<

client_socket.o: ${CLIENT_DIR}/client_socket.c ${CLIENT_DIR}/client_lib.h
	$(CC) -c $<

client_lib.o: ${CLIENT_DIR}/client_lib.c ${CLIENT_DIR}/client_lib.h
	$(CC) -c $<

clean:
	rm ${SERVER_DIR}/server_socket.o ${SERVER_DIR}/server_lib.o ${CLIENT_DIR}/client_socket.o ${CLIENT_DIR}/client_lib.o ${BIN_DIR}/*
