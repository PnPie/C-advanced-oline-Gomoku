CC = gcc

CFLAFS_COMPILE = -c -Wall

CLIENT_DIR = ./src/client
SERVER_DIR = ./src/server
BIN_DIR = ./bin

SERVER_EXE = ${BIN_DIR}/server
CLIENT_EXE = ${BIN_DIR}/client

SERVER_SRC_SOCK = ${SERVER_DIR}/server_socket.c ${SERVER_DIR}/server_lib.h
SERVER_SRC_LIB  = ${SERVER_DIR}/server_lib.c ${SERVER_DIR}/server_lib.h
CLIENT_SRC_SOCK = ${CLIENT_DIR}/client_socket.c ${CLIENT_DIR}/client_lib.h
CLIENT_SRC_LIB  = ${CLIENT_DIR}/client_lib.c ${CLIENT_DIR}/client_lib.h

SERVER_OBJ = ${SERVER_DIR}/server_socket.o ${SERVER_DIR}/server_lib.o
CLIENT_OBJ = ${CLIENT_DIR}/client_socket.o ${CLIENT_DIR}/client_lib.o

all: $(SERVER_EXE) $(CLIENT_EXE)

$(SERVER_EXE): $(SERVER_OBJ)
	$(CC) -o $@ $^

$(CLIENT_EXE): $(CLIENT_OBJ)
	$(CC) -o $@ $^

server_socket.o: $(SERVER_SRC_SOCK)
	$(CC) $(CFLAFS_COMPILE) $<

server_lib.o: $(SERVER_SRC_LIB)
	$(CC) $(CFLAFS_COMPILE) $<

client_socket.o: $(CLIENT_SRC_SOCK)
	$(CC) $(CFLAFS_COMPILE) $<

client_lib.o: $(CLIENT_SRC_LIB)
	$(CC) $(CFLAFS_COMPILE) $<

clean:
	rm $(SERVER_OBJ) $(CLIENT_OBJ) ${BIN_DIR}/*
