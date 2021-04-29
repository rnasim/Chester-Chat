//
//  Server.c
//  Chess Chat Server
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "Board.h"
#include "Pieces.h"
#include "Conditions.h"
#include "FriendsList.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int clients[500];
char *usernames[100];
char *chatusernames[100];
int numclients = 0;
int ChessIsRunning[100];
BOARD *boards[100];
typedef struct ChessInfo CINFO;
struct ChessInfo
{
   int socket;
   PLAYER *player;
};

int Register(char *username, char *password, int socket)
{
   FILE *clientlist = fopen("UsernamesPasswords.txt", "r");
   char string[1000];
   int counter = 1;
   while(fgets(string, sizeof(string), clientlist) != NULL)
   {
      strtok(string, "\n");
      if(counter%2 != 0 && strcmp(string, username) == 0)
      {
         return 1;
      }
      counter++;
   }
   fclose(clientlist);
   clientlist = fopen("UsernamesPasswords.txt", "a");
   fprintf(clientlist, "%s\n", username);
   fprintf(clientlist, "%s\n", password);
   fclose(clientlist);
   return 0;
}
int Login(char *username, char *password, int socket)
{
   FILE *file = fopen("UsernamesPasswords.txt", "r");
   char line[1000];
   int counter = 1;
   while(fgets(line, sizeof(line), file) != NULL)
   {
      strtok(line, "\n");
      if(counter%2 != 0 && strcmp(line, username) == 0)
      {
         fgets(line, sizeof(line), file);
         strtok(line, "\n");
         if(strcmp(line, password) == 0)
         {
            fclose(file);
            return 0;
         }
         else
         {
            fclose(file);
            return 1;
         }
         
      }
      counter++;
   }
   fclose(file);
   return 2;
}
void FatalError(const char *errormessage)
{
   puts(errormessage);
   exit(10);
}
int FindSocket(char *username)
{
   for(int i = 0; i < 100; i++)
   {
      if(usernames[i] != NULL)
      {
         if(strcmp(usernames[i], username) == 0)
         {
            return i;
         }
      }
   }
   return -1;
}
void SendtoAll(void *message, int sockfd)
{
   pthread_mutex_lock(&mutex);
   for(int i = 0; i < numclients; i++)
   {
      if(clients[i] != sockfd)
      {
         if(write(clients[i], message, strlen(message)) < 0)
         {
            FatalError("Could not send message.");
            continue;
         }
      }
   }
   pthread_mutex_unlock(&mutex);
}
void *Communicate(void *sockfd)
{
   int socket = *((int *)sockfd);
   int currentclient = 0;
   char recvmsg[1000], *username = NULL, *password = NULL, *chatusername = NULL;
   int destsocket = -2;
   unsigned long int separator;
   int row_src, row_dest, col_src, col_dest;
   CINFO *white = malloc(sizeof(CINFO));
   CINFO *black = malloc(sizeof(CINFO));
   char *starttag = "#START#";
   char *chesstag = "#CHESS#";
   while(read(socket, recvmsg, sizeof(recvmsg)) > 0)
   {
      if(destsocket == -1)
      {
          destsocket = FindSocket(chatusernames[socket]);
      }
      if(recvmsg[0] == '#' && recvmsg[1] == 'C' && recvmsg[2] == 'H' && recvmsg[3] == 'A' && recvmsg[4] == 'T' && recvmsg[5] == '#' && recvmsg[6] == '\n')
      {
         char *token = strtok(recvmsg, "\n");
         int counter = 0;
         while(counter < 1)
         {
            token = strtok(NULL, "\n");
            if(counter == 0)
            {
               chatusername = token;
               chatusernames[socket] = (char*)malloc(sizeof(token));
               strcpy(chatusernames[socket], token);
            }
            counter++;
            destsocket = FindSocket(chatusernames[socket]);
         }
         continue;
      }
      if(recvmsg[0] == '#' && recvmsg[1] == 'R' && recvmsg[2] == 'E' && recvmsg[3] == 'G' && recvmsg[4] == '#' && recvmsg[5] == '\n')
      {
         char *token = strtok(recvmsg, "\n");
         int counter = 0;
         while(counter < 2)
         {
            token = strtok(NULL, "\n");
            if(counter == 0)
            {
               username = token;
               usernames[socket] = (char*)malloc(sizeof(token));
               strcpy(usernames[socket], token);
            }
            else if(counter == 1)
            {
               password = token;
            }
            counter++;
         }
         int reg = Register(username, password, socket);
         if(reg == 1)
         {
            write(socket, "#REGERROR#", sizeof("#REGERROR#"));
            usernames[socket] = NULL;
         }
         if(reg == 0)
         {
            write(socket, "#REGSUCCESS#", sizeof("#REGSUCCESS#"));
         }
         continue;
      }
      if(recvmsg[0] == '#' && recvmsg[1] == 'L' && recvmsg[2] == 'O' && recvmsg[3] == 'G' && recvmsg[4] == 'I' && recvmsg[5] == 'N' && recvmsg[6] == '#' && recvmsg[7] == '\n')
      {
         char *token = strtok(recvmsg, "\n");
         int counter = 0;
         while(counter < 2)
         {
            token = strtok(NULL, "\n");
            if(counter == 0)
            {
               username = token;
               usernames[socket] = (char*)malloc(sizeof(token));
               strcpy(usernames[socket], token);
            }
            else if(counter == 1)
            {
               password = token;
            }
            counter++;
         }

         int login = Login(username, password, socket);
         if(login == 1)
         {
            write(socket, "#LOGINERROR1#", sizeof("#LOGINERROR1#"));
            usernames[socket] = NULL;
         }
         if(login == 2)
         {
            write(socket, "#LOGINERROR2#", sizeof("#LOGINERROR2#"));
             usernames[socket] = NULL;
         }
         if(login == 0)
         {
            write(socket, "#LOGINSUCCESS#", sizeof("#LOGINSUCCESS#"));
         }
         continue;
      }
      //printf("Socket %d sent a message.\n", socket);
      separator = strcspn(recvmsg, ":");
      if((recvmsg[separator+2] == 'C' || recvmsg[separator+2] == 'c') && recvmsg[separator+3] == 'h' && recvmsg[separator+4] == 'e' && recvmsg[separator+5] == 's' && recvmsg[separator+6] == 's' && ChessIsRunning[socket] == 0)
      {
         ChessIsRunning[socket] = 1;
        ChessIsRunning[destsocket] = 1;
         write(socket, starttag, sizeof(starttag));
         write(destsocket, starttag, sizeof(starttag));
         boards[socket] = chess();
         boards[destsocket] = boards[socket];
         white->socket = socket;
         white->player = boards[socket]->white;
         black->player = boards[socket]->black;
      }
      if(ChessIsRunning[socket] == 2 && socket != white->socket && strlen(recvmsg)-separator-3 == 4) // black move
      {
         recvmsg[strlen(recvmsg)] = '\0';
         if(strlen(recvmsg)-separator-3 == 4)
         {
            black->socket = socket;
            col_src = AlphatoNum(recvmsg[separator+2]);
            row_src = (int)(recvmsg[separator+3]) - 48;
            col_dest = AlphatoNum(recvmsg[separator+4]);
            row_dest = (int)(recvmsg[separator+5]) - 48;
            PIECE *piece = CheckPiece(boards[socket]->black, row_src, col_src);
            if(piece == NULL)
            {
               write(socket, "Illegal move. Try again.\n", sizeof("Illegal move. Try again.\n"));
               continue;
            }
            if(CallPiece(boards[socket], boards[socket]->white, piece, row_src, col_src, row_dest, col_dest, 1) == 1)
            {
               write(socket, "Illegal move. Try again.\n", sizeof("Illegal move. Try again.\n"));
               continue;
            }
            MovePiece(boards[socket], boards[socket]->white, piece, row_dest-1, col_dest-1, 0);
            write(socket, chesstag, sizeof(chesstag));
            write(destsocket, chesstag, sizeof(chesstag));
            write(socket, recvmsg, sizeof(recvmsg));
            recvmsg[strlen(recvmsg)] = '\0';
            write(destsocket, recvmsg, sizeof(recvmsg));
            continue;
         }
      }
      else if(ChessIsRunning[socket] == 2 && socket == white->socket && strlen(recvmsg)-separator-3 == 4) // white move
      {
         col_src = AlphatoNum(recvmsg[separator+2]);
         row_src = (int)(recvmsg[separator+3]) - 48;
         col_dest = AlphatoNum(recvmsg[separator+4]);
         row_dest = (int)(recvmsg[separator+5]) - 48;
         PIECE *piece = CheckPiece(boards[socket]->white, row_src, col_src);
         if(piece == NULL)
         {
            write(socket, "Illegal move. Try again.\n", sizeof("Illegal move. Try again.\n"));
            continue;
         }
         if(CallPiece(boards[socket], boards[socket]->black, piece, row_src, col_src, row_dest, col_dest, 1) == 1)
         {
            write(socket, "Illegal move. Try again.\n", sizeof("Illegal move. Try again.\n"));
            continue;
         }
         MovePiece(boards[socket], boards[socket]->black, piece, row_dest-1, col_dest-1, 0);
         write(socket, chesstag, sizeof(chesstag));
         //Send(chesstag, socket);
         write(destsocket, chesstag, sizeof(chesstag));
         write(socket, recvmsg, sizeof(recvmsg));
         recvmsg[strlen(recvmsg)] = '\0';
         //Send(recvmsg, socket);
          write(destsocket, recvmsg, sizeof(recvmsg));
         continue;
      }
      recvmsg[strlen(recvmsg)] = '\0';
      if(destsocket > 0)
      {
          write(destsocket, recvmsg, sizeof(recvmsg));
      }
      //Send(recvmsg, socket);
      memset(recvmsg, '\0', sizeof(recvmsg));
      if(ChessIsRunning[socket] == 1)
      {
         ChessIsRunning[socket] = 2;
        ChessIsRunning[destsocket] = 2;

      }
      
   }
   pthread_mutex_lock(&mutex);
   for(int i = 0; i < numclients; i++)
   {
      if(clients[i] == socket)
      {
         currentclient = i;
         while(currentclient < numclients-1)
         {
            clients[currentclient] = clients[currentclient+1];
            currentclient++;
         }
      }
   }
   numclients--;
   pthread_mutex_unlock(&mutex);
   return NULL;
}
int MakeServerSocket(uint16_t port)
{
   int sockfd;
   struct sockaddr_in ServerSocketName;
   sockfd = socket(PF_INET, SOCK_STREAM, 0);
   memset(ServerSocketName.sin_zero,'\0',sizeof(ServerSocketName.sin_zero));
   if(sockfd < 0)
   {
      FatalError("Could not create socket.");
   }
   ServerSocketName.sin_family = AF_INET;
   ServerSocketName.sin_port = htons(port);
   ServerSocketName.sin_addr.s_addr = htonl(INADDR_ANY);
   if(bind(sockfd, (struct sockaddr*)&ServerSocketName, sizeof(ServerSocketName)) < 0)
   {
      FatalError("Could not bind the server to this socket.");
   }
   if(listen(sockfd, 5) < 0)
   {
      FatalError("Could not listen on this socket.");
   }
   return sockfd;
}
int main(int argc, char *argv[])
{
   socklen_t ClientLen;
   struct sockaddr_in ClientAddress;
   pthread_t receiving;
   int port = 0, sockfd = 0, newsockfd = 0;
   if(argc < 2)
   {
      FatalError("Please provide a port number!");
   }
   port = atoi(argv[1]);
   if(port <= 2000)
   {
      FatalError("Port number should be > 2000.");
   }
   sockfd = MakeServerSocket(port);
   printf("%d", sockfd);
   //PLIST *packetlist = malloc(sizeof(PLIST));
   //packetlist->length = 0;
   while(1)
   {
      ClientLen = sizeof(ClientAddress);
      newsockfd = accept(sockfd, (struct sockaddr*)&ClientAddress, &ClientLen);
      if(newsockfd < 0)
      {
         FatalError("Could not create data socket.");
      }
      pthread_mutex_lock(&mutex);
      char ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, (struct sockaddr *)&ClientAddress, ip, INET_ADDRSTRLEN);
      clients[numclients] = newsockfd;
      printf("%s connected\n", ip);
      numclients++;
      pthread_create(&receiving, NULL, Communicate, &newsockfd);
      pthread_mutex_unlock(&mutex);
   }
   return 0;
}


