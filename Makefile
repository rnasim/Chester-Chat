# Makefile for Chesster, EECS22L Winter 2019
# Author: Marvis Nguyen
# Modified by: Marvis Nguyen

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0 gthread-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0 gthread-2.0`

INC	= $(GTKINC)
LIBS	= $(GTKLIBS)

OPTS = -g
CC = gcc
DEBUG = -g -DDEBUG
CFLAGS = -ansi -Wall -c -std=c99
# LFLAGS: -Wall
#ASCII: bin/Chess clean

all: bin/ServerSelect bin/Client bin/Server bin/ChessterChatGUI clean
Client: bin/Client clean
Server: bin/Server bin/GUIServer clean
GUI: bin/ChessterChatGUI clean


bin/Client: bin/Client.o bin/Board.o bin/Pieces.o bin/Movegen.o bin/Conditions.o bin/FriendsList.o 
	$(CC)  bin/Board.o bin/Client.o bin/Pieces.o bin/Movegen.o bin/Conditions.o bin/FriendsList.o $(OPTS) -pthread -o bin/Client

bin/Server: bin/Server.o bin/Board.o bin/Pieces.o bin/Movegen.o bin/Conditions.o 
	$(CC) bin/Board.o bin/Server.o bin/Pieces.o bin/Movegen.o bin/Conditions.o $(OPTS) -pthread -o bin/Server
	
bin/GUIServer: bin/GUIServer.o bin/Board.o bin/Pieces.o bin/Movegen.o bin/Conditions.o 
	$(CC) bin/Board.o bin/GUIServer.o bin/Pieces.o bin/Movegen.o bin/Conditions.o $(OPTS) -pthread -o bin/GUIServer

bin/ChessterChatGUI: bin/chatGUI.o bin/list.o bin/ChatWindow.o bin/ClientFunctions.o bin/FriendsList.o bin/GTK_chess.o
	$(CC) $(LFLAGS) bin/chatGUI.o bin/list.o bin/ChatWindow.o bin/ClientFunctions.o bin/FriendsList.o bin/GTK_chess.o $(LIBS) $(OPTS) -o bin/ChessterChatGUI

bin/FriendsList.o: src/FriendsList.c src/FriendsList.h
	$(CC) $(OPTS) $(CFLAGS) src/FriendsList.c -o bin/FriendsList.o
	
bin/Board.o: src/Board.c src/Board.h src/Pieces.h src/Movegen.h src/Conditions.h
	$(CC) $(CFLAGS) src/Board.c -o bin/Board.o

bin/Client.o: src/Client.c src/Board.h
	$(CC) $(OPTS) $(CFLAGS) src/Client.c -pthread -o bin/Client.o

bin/Server.o: src/Server.c src/Board.h
	$(CC) $(OPTS) $(CFLAGS) src/Server.c -pthread -o bin/Server.o

bin/GUIServer.o: src/GUIServer.c src/Board.h	
	$(CC) $(OPTS) $(CFLAGS) src/GUIServer.c -pthread -o bin/GUIServer.o


bin/Pieces.o: src/Pieces.c src/Pieces.h src/Board.h src/Conditions.h
	$(CC) $(CFLAGS) src/Pieces.c -o bin/Pieces.o

bin/Movegen.o: src/Movegen.c src/Movegen.h src/Pieces.h src/Board.h src/Conditions.h
	$(CC) $(CFLAGS) src/Movegen.c -o bin/Movegen.o

bin/Conditions.o: src/Conditions.c src/Conditions.h src/Board.h src/Pieces.h
	$(CC) $(CFLAGS) src/Conditions.c -o bin/Conditions.o


bin/GTK_chess.o: src/GTK_chess.c src/GTK_chess.h
	$(CC) $(CFLAGS) src/GTK_chess.c $(INC) $(OPTS) -o bin/GTK_chess.o

bin/ClientFunctions.o: src/ClientFunctions.c src/ClientFunctions.h 
	$(CC) $(OPTS) $(CFLAGS) src/ClientFunctions.c -pthread -o bin/ClientFunctions.o

bin/ChatWindow.o: src/ChatWindow.c src/ChatWindow.h
	$(CC) $(CFLAGS) src/ChatWindow.c $(INC) $(OPTS) -o bin/ChatWindow.o

bin/list.o: src/list.c src/list.h src/FriendsList.h
	$(CC) $(CFLAGS) src/list.c $(INC) $(OPTS) -o bin/list.o

bin/chatGUI.o: src/chatGUI.c src/chatGUI.h
	$(CC) $(CFLAGS) src/chatGUI.c $(INC) $(OPTS) -o bin/chatGUI.o


clean:
	rm -f bin/*.o $(GUI)

tar:
	tar -czvf Chess_V1.0_src.tar.gz src
	

