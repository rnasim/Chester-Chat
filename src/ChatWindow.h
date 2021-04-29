#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef struct Data DATA;
typedef struct Chat CHAT; 


struct Data {
	GtkWidget* table;
	GtkWidget* chat_entry;
	GtkWidget* main_window;
	int current_location;
	int user;
	int Shutdown;
	int socket;
    char *Name;
};

void GetUsername(char *u);
void CallChatWindow(char *chatFriend, int sockfd);
void Send_Message(GtkWidget* button, gpointer g_data );
void Send_Chess(GtkWidget *widget, gpointer g_data);
GtkWidget *Chat(DATA* orig_data);
void *Chat_Receive(gpointer g_data);
void Chat_Loop(int ServSocketFD);
GtkWidget *Time_Stamp(void);
void Print_Recieved(DATA *g_data);
void Update_Chat(void);

#endif
