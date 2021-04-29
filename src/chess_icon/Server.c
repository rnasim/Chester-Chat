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
int numclients = 0;
int ChessIsRunning = 0;
typedef struct ChessInfo CINFO;
typedef struct Package PACK;
typedef struct PackageList PLIST;
struct ChessInfo
{
   int socket;
   PLAYER *player;
};
struct PackageList
{
   PACK *first;
   PACK *last;
   int length;
};
struct Package
{
   int sockfd;
   char *username;
   PACK *next;
   PACK *prev;
   PLIST *list;
};
BOARD *board;

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
int FindSocket(char *username, PLIST *list)
{
   PACK *curr = list->first;
   while(curr != NULL)
   {
      if(strcmp(curr->username, username) == 0)
      {
         return curr->sockfd;
      }
      curr = curr->next;
   }
   return -1;
}
PACK *AddPacketEntry(PLIST *list)
{
   PACK *packet = malloc(sizeof(PACK));
   packet->list = list;
   packet->username = NULL;
   if(list->first == NULL)
   {
      list->first = packet;
      list->last = packet;
      packet->next = NULL;
      packet->prev = NULL;
   }
   else
   {
      packet->prev = list->last;
      packet->next = NULL;
      list->last->next = packet;
      list->last = packet;
   }
   list->length++;
   return packet;
}
void Send(void *message, int sockfd)
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
void *Communicate(void *packet)
{
   PACK *p = (PACK *) packet;
   int socket = p->sockfd;
   int currentclient = 0;
   char recvmsg[1000], *username = NULL, *password = NULL, *chatusername = NULL;
   unsigned long int separator;
   int row_src, row_dest, col_src, col_dest;
   CINFO *white = malloc(sizeof(CINFO));
   CINFO *black = malloc(sizeof(CINFO));
   char *starttag = "#START#";
   char *chesstag = "#CHESS#";
   bool flag = false;
   while(read(socket, recvmsg, sizeof(recvmsg)) > 0)
   {
      if(recvmsg[0] == '#' && recvmsg[1] == 'C' && recvmsg[2] == 'H' && recvmsg[3] == 'A' && recvmsg[4] == 'T' && recvmsg[5] == '#')
      {
         char *token = strtok(recvmsg, "\n");
         int counter = 0;
         while(counter < 2)
         {
            token = strtok(NULL, "\n");
            if(counter == 0)
            {
               chatusername = token;
            }
            else if(counter == 1)
            {
               p->username = token;
            }
            counter++;
         }
         flag = true;
         continue;
      }
      if(flag == true)
      {
         if(FindSocket(chatusername, p->list) == -1)
         {
            continue;
         }
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
         }
         if(login == 2)
         {
            write(socket, "#LOGINERROR2#", sizeof("#LOGINERROR2#"));
         }
         if(login == 0)
         {
            write(socket, "#LOGINSUCCESS#", sizeof("#LOGINSUCCESS#"));
         }
         continue;
      }
      printf("Socket %d sent a message.\n", socket);
      separator = strcspn(recvmsg, ":");
      if((recvmsg[separator+2] == 'C' || recvmsg[separator+2] == 'c') && recvmsg[separator+3] == 'h' && recvmsg[separator+4] == 'e' && recvmsg[separator+5] == 's' && recvmsg[separator+6] == 's' && ChessIsRunning == 0)
      {
         ChessIsRunning = 1;
         write(socket, starttag, sizeof(starttag));
         Send(starttag, socket);
         board = chess();
         white->socket = socket;
         white->player = board->white;
         black->player = board->black;
      }
      if(ChessIsRunning == 2 && socket != white->socket) // black move
      {
         recvmsg[strlen(recvmsg)] = '\0';
         if(strlen(recvmsg)-separator-3 == 4)
         {
            black->socket = socket;
            col_src = AlphatoNum(recvmsg[separator+2]);
            row_src = (int)(recvmsg[separator+3]) - 48;
            col_dest = AlphatoNum(recvmsg[separator+4]);
            row_dest = (int)(recvmsg[separator+5]) - 48;
            PIECE *piece = CheckPiece(board->black, row_src, col_src);
            if(piece == NULL)
            {
               write(socket, "Illegal move. Try again.\n", sizeof("Illegal move. Try again.\n"));
               continue;
            }
            if(CallPiece(board, board->white, piece, row_src, col_src, row_dest, col_dest, 1) == 1)
            {
               write(socket, "Illegal move. Try again.\n", sizeof("Illegal move. Try again.\n"));
               continue;
            }
            MovePiece(board, board->white, piece, row_dest-1, col_dest-1, 0);
            write(socket, chesstag, sizeof(chesstag));
            Send(chesstag, socket);
            write(socket, recvmsg, sizeof(recvmsg));
            recvmsg[strlen(recvmsg)] = '\0';
            Send(recvmsg, socket);
            continue;
         }
      }
      else if(ChessIsRunning == 2 && socket == white->socket && strlen(recvmsg)-separator-3 == 4) // white move
      {
         col_src = AlphatoNum(recvmsg[separator+2]);
         row_src = (int)(recvmsg[separator+3]) - 48;
         col_dest = AlphatoNum(recvmsg[separator+4]);
         row_dest = (int)(recvmsg[separator+5]) - 48;
         PIECE *piece = CheckPiece(board->white, row_src, col_src);
         if(piece == NULL)
         {
            write(socket, "Illegal move. Try again.\n", sizeof("Illegal move. Try again.\n"));
            continue;
         }
         if(CallPiece(board, board->black, piece, row_src, col_src, row_dest, col_dest, 1) == 1)
         {
            write(socket, "Illegal move. Try again.\n", sizeof("Illegal move. Try again.\n"));
            continue;
         }
         MovePiece(board, board->black, piece, row_dest-1, col_dest-1, 0);
         write(socket, chesstag, sizeof(chesstag));
         Send(chesstag, socket);
         write(socket, recvmsg, sizeof(recvmsg));
         recvmsg[strlen(recvmsg)] = '\0';
         Send(recvmsg, socket);
         continue;
      }
      recvmsg[strlen(recvmsg)] = '\0';
      Send(recvmsg, socket);
      memset(recvmsg, '\0', sizeof(recvmsg));
      if(ChessIsRunning == 1)
      {
         ChessIsRunning = 2;
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
   PLIST *packetlist = malloc(sizeof(PLIST));
   packetlist->length = 0;
   while(1)
   {
      ClientLen = sizeof(ClientAddress);
      newsockfd = accept(sockfd, (struct sockaddr*)&ClientAddress, &ClientLen);
      if(newsockfd < 0)
      {
         FatalError("Could not create data socket.");
      }
      pthread_mutex_lock(&mutex);
      PACK *packet = AddPacketEntry(packetlist);
      char ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, (struct sockaddr *)&ClientAddress, ip, INET_ADDRSTRLEN);
      clients[numclients] = newsockfd;
      printf("%s connected\n", ip);
      numclients++;
      packet->sockfd = newsockfd;
      pthread_create(&receiving, NULL, Communicate, (void *)packet);
      pthread_mutex_unlock(&mutex);
   }
   return 0;
}


