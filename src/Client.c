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
#include "FriendsList.h"
#include "Board.h"

<<<<<<< HEAD
char encrypt(char message[1000])
{
	for (int i = 0; (i < 1000 && message[i] != '\0'); i++)
	{
		message[i] = message[i] + 2; //2 added to ASCII value  
	}
	printf ("This is the encrypted message: \n %s\n", message);
	return message;
}

char decrypt(char message[1000])
{
	for (int i = 0; (i < 1000 && message[i] != '\0'); i++)
	{
		message[i] = message[i] - 2; // 2 Subtracted to ASCII value
	}
	printf("This is the decrypted message: \n %s \n", message);
	return message;
}

=======
char *boardarray[8][8];
>>>>>>> b252ec4952b1915e6e693fb613537728b260a465
void FatalError(const char *errormessage)
{
    puts(errormessage);
    exit(0);
}
void PrintFriendsList(char *username)
{
    strtok(username, "\n");
    char string[1000];
    char *filename = GetFriendListName(username);
    FILE *file = fopen(filename, "a+");
    printf("*** Your friends *** \n");
    int numfriends = 0;
    while(fgets(string, sizeof(string), file) != NULL)
    {
        strtok(string, "\n");
        printf("%s\n", string);
        numfriends++;
    }
    if(numfriends == 0)
    {
        printf("New friends will be listed once you add some!\n");
    }
    fclose(file);
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
<<<<<<< HEAD
	decrypt(recv);
        recv[strlen(recv)] = '\0';
        fputs(recv, stdout);
=======
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
>>>>>>> b252ec4952b1915e6e693fb613537728b260a465
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
    char send[1000], message[1000], auth[1000], buffer[1000], username[1000], password[1000], key[1000], friendusername[1000], chat[1000], init[1000];
    bool StartChatting = false;
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
    printf("\nConnected! \n\n");
    PrintFriendsList(username);
    printf("\n");
    printf("To add a friend, type Add. \n");
    printf("To remove a friend, type Remove. \n");
    printf("To view your friends list again, type View. \n");
    printf("To chat with a friend, type Chat. \n");
    printf("To exit, type Exit. \n");
    scanf("%s", key);
    getchar();
    strtok(username, "\n");
    while(strcmp(key, "Chat") != 0)
    {
        if(strcmp(key, "chat") == 0)
        {
            break;
        }
        if(strcmp(key, "ADD") == 0 || strcmp(key, "Add") == 0 || strcmp(key, "add") == 0) // add a friend
        {
            printf("Enter the username of the friend you wish to add: ");
            scanf("%s", friendusername);
            char *yourfriendslist = GetFriendListName(username);
            char *friendsfriendslist =  GetFriendListName(friendusername);
            FILE *friendsfile = fopen(friendsfriendslist, "a+");
            FILE *yourfile = fopen(yourfriendslist, "a+");
            int add = AddFriend(friendusername, username, friendsfile, yourfile);
            if(add == 0)
            {
                printf("Friend successfully added! \n");
            }
            printf("\n");
        }
        if(strcmp(key, "REM") == 0 || strcmp(key, "Rem") == 0 || strcmp(key, "rem") == 0 || strcmp(key, "Remove") == 0 || strcmp(key, "remove") == 0) // remove a friend
        {
            printf("Enter the username of the friend you wish to remove: ");
            scanf("%s", friendusername);
            char *yourfriendslist = GetFriendListName(username);
            char *friendsfriendslist =  GetFriendListName(friendusername);
            FILE *yourfile = fopen(yourfriendslist, "r+");
            int remove = RemoveFriend(friendusername, username, yourfile);
            if(remove == 0)
            {
                FILE *friendsfile = fopen(friendsfriendslist, "r+");
                remove = RemoveFriend(username, friendusername, friendsfile);
                fclose(yourfile);
                fclose(friendsfile);
                printf("Friend successfully removed! \n");
            }
            printf("\n");
        }
        if(strcmp(key, "VIEW") == 0 || strcmp(key, "View") == 0 || strcmp(key, "view") == 0) // view friends list
        {
            printf("\n");
            PrintFriendsList(username);
            printf("\n");
        }
        if(strcmp(key, "Exit") == 0 || strcmp(key, "exit") == 0) { // exit program entirely
            exit(10);
        }
        printf("To add a friend, type Add. \n");
        printf("To remove a friend, type Remove. \n");
        printf("To view your friends list again, type View. \n");
        printf("To chat with a friend, type Chat. \n");
        scanf("%s", key);
        getchar();
        
    }
    /*check to see if they are friends with the person they want to chat with*/
    char *yourlistname = GetFriendListName(username);
    FILE *friendlist = fopen(yourlistname, "r+");
    char string[100];
    int counter = 0;
    char *friends[100];
    int numfriends;
    for(numfriends = 0; fgets(string, sizeof(string), friendlist) != NULL; numfriends++)
    {
        strtok(string, "\n");
        friends[numfriends] = (char*)malloc(sizeof(string));
        strcpy(friends[numfriends], string);
    }
    while(StartChatting == false)
    {
        printf("Enter the user you wish to chat with: ");
        scanf("%s", chat);
        getchar();
        for(int i = 0; i < numfriends; i++)
        {
            if(strcmp(friends[i], chat) == 0)
            {
                StartChatting = true;
                break;
            }
            counter++;
        }
       if(numfriends == counter)
       {
           printf("You are not friends with that person! Please request to be their friend before chatting with them. \n");
       }
        counter = 0;
    }
    strcpy(init, "#CHAT#\n");
    strcat(init, chat);
    u_int32_t userlen = htonl(strlen(init));
    write(sockfd, &userlen, sizeof(userlen));
    write(sockfd, init, strlen(init));
    pthread_create(&receiving, NULL, receive, &sockfd);
    while(fgets(message, 1000, stdin) > 0)
    {
<<<<<<< HEAD
	encrypt(message);
=======
        if(strcmp(message, "Exit\n") == 0)
        {
            char exitmessage[1000];
            strcpy(exitmessage, username);
            strcat(exitmessage, " has left the chat\n");
            u_int32_t len = htonl(strlen(exitmessage));
            write(sockfd, &len, sizeof(len));
            write(sockfd, exitmessage, strlen(exitmessage));
            exit(10);
        }
>>>>>>> b252ec4952b1915e6e693fb613537728b260a465
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
