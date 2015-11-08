CC = gcc -Wall

all: serveur client

serveur: comserv.o gestserv.o
	$(CC) -o serveur $^

client: comcli.o gestcli.o
	$(CC) -o client $^

comserv.o: comserv.c protoserv.h
	$(CC) -c $<

gestserv.o: gestserv.c protoserv.h
	$(CC) -c $<

comcli.o: comcli.c protoclient.h
	$(CC) -c $<

gestcli.o: gestcli.c protoclient.h
	$(CC) -c $<

clean:
	rm comserv.o gestserv.o comcli.o gestcli.o