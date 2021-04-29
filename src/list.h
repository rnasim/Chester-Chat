#ifndef LIST_H
#define LIST_H

#include "ClientFunctions.h"

typedef struct friendList FRIENDLIST;
typedef struct requestList REQUESTLIST;
typedef struct sendData SENDDATA;

struct friendList
{
  GtkWidget *list;
};
struct requestList
{
  GtkWidget *reqList;
};

struct sendData
{
  GtkTreeSelection *selection;
  GtkWidget *entry;
  char *name;
  int sockfd;
};


enum {

  LIST_ITEM = 0,
  N_COLUMNS
};


void init_list(GtkWidget *list);
void addFriend(GtkWidget *widget, gpointer data);
void append_item(GtkWidget *widget, gpointer data);
void on_changed(GtkWidget *widget, gpointer label);
void remove_item(GtkWidget *widget, gpointer selection);
void chatting(GtkWidget *widget, gpointer data);
void friendRequests(char username[MAX_LENGTH]);
void ChatMainMenu(char username[MAX_LENGTH], int sockfd);


#endif

