//
//  Client.c
//  Chess Chat Client
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Board.h"

char *boardarray[8][8];
void FatalError(const char *errormessage)
{
    puts(errormessage);
    exit(0);
}
void *receive(void *sockfd)
{
    int socket = *((int *)sockfd);
    int col_src, row_src, col_dest, row_dest;
    char recv[1000];
    char *boardarray[8][8];
    char move[4];
    while(read(socket, recv, sizeof(recv)) > 0)
    {
        if(strncmp(recv, "#START#", sizeof("#START#")) == 0)
        {
            BOARD *board = chess();
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    boardarray[i][j] = board->boardarray[i][j];
                }
            }
            DrawBoard(boardarray);
        }
        else if(strncmp(recv, "#CHESS#", sizeof("#CHESS#")) == 0)
        {
            read(socket, recv, sizeof(recv));
            unsigned long int separator = strcspn(recv, ":");
            move[0] = recv[separator+2];
            move[1] = recv[separator+3];
            move[2] = recv[separator+4];
            move[3] = recv[separator+5];
            col_src = AlphatoNum(move[0]);
            row_src = (int)(move[1]) - 48;
            col_dest = AlphatoNum(move[2]);
            row_dest = (int)(move[3]) - 48;
            char *temp = boardarray[row_src-1][col_src-1];
            boardarray[row_dest-1][col_dest-1] = temp;
            boardarray[row_src-1][col_src-1] = "  ";
            DrawBoard(boardarray);
        }
        if(strncmp(recv, "#START#", sizeof("#START#")) != 0)
        {
            recv[strlen(recv)] = '\0';
            fputs(recv, stdout);
        }
        memset(recv, '\0', sizeof(recv));
    }
    return NULL;
}
int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in ServerAddress;
    struct hostent *Server;
    pthread_t receiving;
    char option;
    char send[1000], message[1000], auth[1000], buffer[1000], username[1000], password[1000];
    if(argc != 3)
    {
        FatalError("You must have the following arguments in your command: <hostname> <port>");
    }
    Server = gethostbyname(argv[1]);
    strcat(username, "\n");
    if (Server == NULL)
    {
        FatalError("No such server!");
    }
    int port = atoi(argv[2]);
    if(port <= 2000)
    {
        fprintf(stderr, "Invalid port number.");
        exit(10);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        FatalError("Error creating a socket.");
    }
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(port);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];
    if (connect(sockfd, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0)
    {
        FatalError("Could not connect to server.");
    }
    printf("Connected! \n");
    printf("Would you like to register (r) or login (l)? \n");
    scanf(" %c", &option);
    getchar();
    printf("Enter a username: ");
    scanf("%s", username);
    getchar();
    printf("Enter a password: ");
    scanf("%s", password);
    getchar();
    if(option == 'l' || option == 'L')
    {
        strcat(username, "\n");
        strcat(password, "\n");
        strcpy(buffer, "#LOGIN#\n");
        strcat(buffer, username);
        strcat(buffer, password);
        u_int32_t len = htonl(strlen(buffer));
        write(sockfd, &len, sizeof(len));
        write(sockfd, buffer, sizeof(buffer));
        read(sockfd, auth, sizeof(auth));
        if(strcmp(auth, "#LOGINERROR1#") == 0)
        {
             FatalError("The password you entered is incorrect. Please run the program and try again.");
        }
        if(strcmp(auth, "#LOGINERROR2#") == 0)
        {
            FatalError("The username you entered is invalid. Please run the program and try again.");
        }
    }
    else
    {
        strcat(username, "\n");
        strcat(password, "\n");
        strcpy(buffer, "#REG#\n");
        strcat(buffer, username);
        strcat(buffer, password);
        u_int32_t len = htonl(strlen(buffer));
        write(sockfd, &len, sizeof(len));
        write(sockfd, buffer, sizeof(buffer));
        read(sockfd, auth, sizeof(auth));
        if(strcmp(auth, "#REGERROR#") == 0)
        {
            FatalError("The username you entered has already been taken. Please run the program and try again.");
        }
    }
	printf("Connected! \n");
	printf("To find out how to start chatting with friends, type 'help'\n");
	char *help;
	scanf(%s, help);
	if((strcmp('help', help))=0){
		printf("If you want to add a friend, type ADD <username>\n If you want to remove a friend, type REM <username>\n If you want to view your friend list, type FLT \n If you want to view your request list, type RLT\n If you want to accept a friend request, type ACC <username>\n If you want to deny a friend request, type DEN <username>/n If you want to chat with a friend, type CHAT <username>\n");
	}
	char input[1000];
	scanf(%s, input);
	char key[1000];
	char friendusername[1000];
	for(int i = 0; i < 3; i++){
		key[i] = input[i]
	} 
	if(strcmp(key, 'ADD')==0) {
		for(int i = 3; i < sizeof(input); i++){
		friendusername[i] = input[i]
		}
		if(strcmp('ADD', key) == 0) {
		strcpy(buffer, "#ADD#\n");
		strcat(buffer, friendusername)
		strcat(buffer, username);
		u_int32_t len = htonl(strlen(buffer));
        write(sockfd, &len, sizeof(len));
        write(sockfd, buffer, sizeof(buffer));
        read(sockfd, auth, sizeof(auth));
		}
	}
	if(strcmp(key, 'REM')==0) {
		for(int i = 3; i < sizeof(input); i++){
		friendusername[i] = input[i]
		}
		if(strcmp('REM', key) == 0) {
		strcpy(buffer, "#REM#\n");
		strcat(buffer, friendusername)
		strcat(buffer, username);
		u_int32_t len = htonl(strlen(buffer));
        write(sockfd, &len, sizeof(len));
        write(sockfd, buffer, sizeof(buffer));
        read(sockfd, auth, sizeof(auth));
		}
	}
	if(strcmp(key, 'FLT')==0) {
		if(strcmp('FLT', key) == 0) {
		strcpy(buffer, "#FLT#\n");
		strcat(buffer, username);
		u_int32_t len = htonl(strlen(buffer));
        write(sockfd, &len, sizeof(len));
        write(sockfd, buffer, sizeof(buffer));
        read(sockfd, auth, sizeof(auth));
		}
	}
	if(strcmp(key, 'RLT')==0) {
		if(strcmp('FLT', key) == 0) {
		strcpy(buffer, "#FLT#\n");
		strcat(buffer, username);
		u_int32_t len = htonl(strlen(buffer));
        write(sockfd, &len, sizeof(len));
        write(sockfd, buffer, sizeof(buffer));
        read(sockfd, auth, sizeof(auth));
		}
	}
	if(strcmp(key, 'ACC')==0) {
		for(int i = 3; i < sizeof(input); i++){
		friendusername[i] = input[i]
		}
		if(strcmp('ACC', key) == 0) {
		strcpy(buffer, "#ACC#\n");
		strcat(buffer, friendusername)
		strcat(buffer, username);
		u_int32_t len = htonl(strlen(buffer));
        write(sockfd, &len, sizeof(len));
        write(sockfd, buffer, sizeof(buffer));
        read(sockfd, auth, sizeof(auth));
		}
	}
	if(strcmp(key, 'DEN')==0) {
		for(int i = 3; i < sizeof(input); i++){
		friendusername[i] = input[i]
		}
		if(strcmp('DEN', key) == 0) {
		strcpy(buffer, "#DEN#\n");
		strcat(buffer, friendusername)
		strcat(buffer, username);
		u_int32_t len = htonl(strlen(buffer));
        write(sockfd, &len, sizeof(len));
        write(sockfd, buffer, sizeof(buffer));
        read(sockfd, auth, sizeof(auth));
		}
	}
    pthread_create(&receiving, NULL, receive, &sockfd);
    strtok(username, "\n");
    while(fgets(message, 1000, stdin) > 0)
    {
        if(strcmp(message, "Exit\n") == 0)
        {
            exit(10);
        }
        strcpy(send, username);
        strcat(send, ": ");
        strcat(send, message);
        u_int32_t len = htonl(strlen(send));
        write(sockfd, &len, sizeof(len));
        if(write(sockfd, send, strlen(send)) < 0)
        {
            FatalError("Message could not reach the server.");
        }
        memset(message,'\0',sizeof(message));
        memset(send,'\0',sizeof(send));
    }
    pthread_join(receiving, NULL);
    close(sockfd);
}
