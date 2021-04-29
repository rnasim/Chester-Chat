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
#include "ClientFunctions.h"

char *boardarray[8][8];
void FatalError(const char *errormessage)
{
    puts(errormessage);
    exit(0);
}
/*void *receive(void *sockfd)
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
}*/

int LoginFunction(char username[MAX_LENGTH], char password[MAX_LENGTH], int sockfd){
    
      char auth[MAX_LENGTH], buffer[MAX_LENGTH];
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
             //FatalError("The password you entered is incorrect. Please run the program and try again.");
             return 1;
        }
        if(strcmp(auth, "#LOGINERROR2#") == 0)
        {
            //FatalError("The username you entered is invalid. Please run the program and try again.");
            return 2;
        }
        return 0;
}

int Registration(char username[MAX_LENGTH], char password[MAX_LENGTH], int sockfd)
{
        char auth[MAX_LENGTH], buffer[MAX_LENGTH];
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
            //FatalError("The username you entered has already been taken. Please run the program and try again.");
            return 1;
        }
        return 0;
}

int MessageClient(char username[MAX_LENGTH], char message[MAX_LENGTH], int sockfd)
{
        char send[MAX_LENGTH];
        strcpy(send, message);
        u_int32_t len = htonl(strlen(send));
        write(sockfd, &len, sizeof(len));
        if(write(sockfd, send, strlen(send)) < 0)
        {
            //FatalError("Message could not reach the server.");
            return 1;
        }
        memset(message,'\0',sizeof(message));
        memset(send,'\0',sizeof(send));
        return 0;
}

/*int main(int argc, char *argv[])
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

//Login
    if(option == 'l' || option == 'L')
    {
	LoginFunction(username, password, sockfd);

/       strcat(username, "\n");
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
//Register
    else
    {
	Registration(username, password, sockfd); 

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

*/

    /*}
    pthread_create(&receiving, NULL, receive, &sockfd);
    strtok(username, "\n");

    ClientMessage(username, message, sockfd);

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
*/

