#include <gtk/gtk.h>
#include <string.h>
#include <assert.h>

#include "list.h"
#include "ChatWindow.h"
#include "FriendsList.h"
#include "ClientFunctions.h"

GtkWidget *list;
GtkWidget *reqList;


void init_list(GtkWidget *list) {
  

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes("List Items",
          renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
  //gtk_tree_view_append_column(GTK_TREE_VIEW(requestList->reqList), column);
  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
      GTK_TREE_MODEL(store));
  //gtk_tree_view_set_model(GTK_TREE_VIEW(requestList->reqList), 
  //    GTK_TREE_MODEL(store));

  g_object_unref(store);
}


void addFriend(GtkWidget *widget, gpointer data){
  SENDDATA *sendData = (SENDDATA*)data;
  char *username = sendData->name;
  const gchar *str = gtk_entry_get_text(GTK_ENTRY(sendData->entry)); 
  char *friendusername = (char *)str;
  
  char *yourfriendslist = GetFriendListName(username);
  char *friendsfriendslist =  GetFriendListName(friendusername);
  FILE *friendsfile = fopen(friendsfriendslist, "a+");
  FILE *yourfile = fopen(yourfriendslist, "a+");
  int add = AddFriend(friendusername, username, friendsfile, yourfile);
  if(add == 0)
  {
    append_item(list, sendData->entry);
    printf("Friend successfully added! \n");
  }
  printf("\n");
}

void append_item(GtkWidget *widget, gpointer entry) {
  
  //FRIENDLIST *friendList = (FRIENDLIST*) entry;  
  
  GtkListStore *store;
  GtkTreeIter iter;
  
  const gchar *str = gtk_entry_get_text(GTK_ENTRY(entry)); 
  
  if (*str != '\0'){
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
   
    gtk_entry_set_text(GTK_ENTRY(entry), "");
  }else {
    g_print("search cannot be empty\n");
  }

}

void on_changed(GtkWidget *widget, gpointer label) {

  //FRIENDLIST *friendList = (FRIENDLIST*) label;  
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
    gtk_label_set_text(GTK_LABEL(label), value);
    g_free(value);
  }
}

void remove_item(GtkWidget *widget, gpointer data) {
  
  //FRIENDLIST *friendList = (FRIENDLIST*) selection;
  SENDDATA *sendData = (SENDDATA*)data;
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;
  gchar *chatFriend;
  

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
  
  
  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
      return;
  }

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(sendData->selection), 
         &model, &iter)) {
    gtk_tree_model_get(model, &iter, LIST_ITEM, &chatFriend, -1);
    char *username = sendData->name;
    char *friendusername = (char *)chatFriend;
  
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
      gtk_list_store_remove(store, &iter);
      printf("Friend successfully removed! \n");
    }
    printf("\n");
  }
}

void chatting(GtkWidget *widget, gpointer data){

  SENDDATA *sendData = (SENDDATA*)data;
  assert(sendData);
  assert(sendData->selection);
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;
  gchar *chatFriend;
  
  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
  
  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
      return;
  }

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(sendData->selection), 
         &model, &iter)) {
      gtk_tree_model_get(model, &iter, LIST_ITEM, &chatFriend, -1);
      g_print("Chatting with %s\n", chatFriend);
      CallChatWindow(chatFriend, sendData->sockfd);
      g_free(chatFriend);
  }
}

void add_to_list(GtkWidget *widget, const gchar *str) {

  
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(reqList)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

void friendRequests(char username[MAX_LENGTH]){

  
  
  GtkWidget *ReqWin;
  GtkWidget *scroller;
  GtkWidget *FriendName = gtk_entry_new();
  //GtkWidget *reqList;
  GtkWidget *accept;
  GtkWidget *decline;
  
  GtkWidget *vbox;
  GtkWidget *hbox;
  
  char string[1000];
  GtkTreeSelection *selection;
  
  ReqWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  gtk_window_set_title(GTK_WINDOW(ReqWin), "Friend Requests");
  gtk_window_set_position(GTK_WINDOW(ReqWin), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER (ReqWin), 10);
  gtk_widget_set_size_request(ReqWin, 370, 270);
  
  scroller = gtk_scrolled_window_new(NULL, NULL);
  
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroller),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroller),
            GTK_SHADOW_ETCHED_IN);
            
  reqList = gtk_tree_view_new();
  //printf("Hello\n");
  //List->reqList = reqList;
  
  gtk_container_add(GTK_CONTAINER(scroller), reqList);
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(reqList), FALSE);
  
  accept = gtk_button_new_with_label("Accept");
  decline = gtk_button_new_with_label("Decline");
  
  vbox = gtk_vbox_new(FALSE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), scroller, TRUE, TRUE, 5);

  hbox = gtk_hbox_new(FALSE, 5);
  
  gtk_box_pack_start(GTK_BOX(hbox), accept, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox), decline, FALSE, TRUE, 3);
  
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 3);
  
  gtk_container_add(GTK_CONTAINER(ReqWin), vbox);
  
  init_list(reqList);
  
  /*const gchar *name = "Benny";
  add_to_list(reqList, name);*/
  
  // adding from reqestlist
  
  char *name = username;
  strtok(name, "\n");
  char *filename = GetRequestListName(name);
  FILE *file = fopen(filename, "a+");
  printf("Filename: %s\n", filename);
  while(fgets(string, sizeof(string), file) != NULL)
  {
        printf("%s", string);
        add_to_list(reqList, string);
        gtk_entry_set_text(GTK_ENTRY(FriendName), string);
  
    g_signal_connect(G_OBJECT(accept), "clicked",
            G_CALLBACK(append_item), FriendName);
  }
    fclose(file);
  
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(reqList));
  

  
  gtk_widget_show_all(ReqWin);
  
  gtk_main();
  //g_free(requestList);
}

void ChatMainMenu(char username[MAX_LENGTH], int sockfd) {


  GtkWidget *window;
  GtkWidget *table;
  GtkWidget *title;
  GtkWidget *scrl;
  //GtkWidget *list;
  //requestList->reqList =  gtk_tree_view_new ();
  GtkWidget *label;
  GtkTreeSelection *selection;
  
  SENDDATA *sendData = g_malloc(sizeof(SENDDATA));
  sendData->sockfd = sockfd;
  sendData->name = username;
  //GtkWidget *wins;
  
  GtkWidget *halign;
  GtkWidget *halign2;
  //GtkWidget *halign3;
  GtkWidget *valign;
  GtkWidget *valign2;
  
  GtkWidget *entry = gtk_entry_new();
  sendData->entry = entry;
  GtkWidget *addBtn;
  GtkWidget *rmBtn;
  GtkWidget *request;
  GtkWidget *chatBtn;
  GtkWidget *logoutBtn;
  
   char string[1000]; //filename[1000];
  
  //gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_widget_set_size_request (window, 400, 350);
  
  gtk_window_set_title(GTK_WINDOW(window), "ChessterChat!");

  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  
  table = gtk_table_new(6, 4, FALSE);
  gtk_table_set_col_spacings(GTK_TABLE(table), 3);
  gtk_table_set_row_spacing(GTK_TABLE(table), 0, 3);

  title = gtk_label_new("Friend List: ");
  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), title);
  gtk_table_attach(GTK_TABLE(table), halign, 0, 1, 0, 1, 
      GTK_FILL, GTK_FILL, 0, 0);

  
  label = gtk_label_new("");
  //list =   //creates listwidget
  list = gtk_tree_view_new ();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
  //hides the header
  init_list(list);   //initialize the list
  
  
  // adding to the friendlist using
  // the text file!!!!!
    //char *username = 'dandog';
    char *name = username;
    strtok(name, "\n");
    char *filename = GetFriendListName(name);
    FILE *file = fopen(filename, "a+");
    printf("Filename: %s\n", filename);
    while(fgets(string, sizeof(string), file) != NULL)
    {
        strtok(string, "\n");
        gtk_entry_set_text(GTK_ENTRY(entry), string);
        printf("%s\n", string);
        append_item(list, entry);
    }
    fclose(file);
  
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
  sendData->selection = selection;
  
  g_signal_connect(selection, "changed", G_CALLBACK(on_changed), label);
  //gtk_container_add(GTK_CONTAINER())
  scrl = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrl), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER(scrl), list);
  
  gtk_table_attach(GTK_TABLE(table), scrl, 0, 2, 1, 3, 
      GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 1, 1);
  
  request = gtk_button_new_with_label("Request(s)");
  gtk_widget_set_size_request(request, 50, 30);
  gtk_table_attach(GTK_TABLE(table), request, 3, 4, 1, 2, 
      GTK_FILL, GTK_SHRINK, 1, 1);
  g_signal_connect(G_OBJECT(request), "clicked", G_CALLBACK(friendRequests), NULL);

  valign = gtk_alignment_new(0, 0, 0, 0);
  rmBtn = gtk_button_new_with_label("Remove");
  g_signal_connect(G_OBJECT(rmBtn), "clicked",
          G_CALLBACK(remove_item), (gpointer)sendData);
  gtk_widget_set_size_request(rmBtn, 70, 30);
  gtk_container_add(GTK_CONTAINER(valign), rmBtn);
  gtk_table_set_row_spacing(GTK_TABLE(table), 1, 3);
  gtk_table_attach(GTK_TABLE(table), valign, 3, 4, 2, 3, 
      GTK_FILL, GTK_FILL | GTK_EXPAND, 1, 1);
      
  // ADD BUTTON
  valign2 = gtk_alignment_new(0,2,0,0);
  addBtn = gtk_button_new_with_label("Add");
  gtk_widget_set_size_request(request, 70, 30);
  gtk_container_add(GTK_CONTAINER(valign2), addBtn);
  gtk_table_attach(GTK_TABLE(table), valign2, 3, 4, 3, 4,
       GTK_FILL, GTK_FILL, 1, 1); 
  

  gtk_widget_set_size_request(entry, 80, 30);
  gtk_table_attach(GTK_TABLE(table), entry, 0, 1, 3, 4,
	GTK_FILL, GTK_FILL, 0, 0); 
  
  //const gchar *requested_userName = gtk_entry_get_text(entry);
  g_signal_connect(G_OBJECT(addBtn), "clicked",
            G_CALLBACK(addFriend), (gpointer)sendData);
    
  g_signal_connect (entry, "activate",
		      G_CALLBACK (addFriend),
		      (gpointer)sendData);

  // CHAT BUTTON
  halign2 = gtk_alignment_new(0, 1, 0, 0);
  chatBtn = gtk_button_new_with_label("Chat");
  gtk_container_add(GTK_CONTAINER(halign2), chatBtn);
  gtk_widget_set_size_request(chatBtn, 70, 30);
  gtk_table_set_row_spacing(GTK_TABLE(table), 3, 5);
  gtk_table_attach(GTK_TABLE(table), halign2, 0, 1, 4, 5, 
      GTK_FILL, GTK_FILL, 0, 0);
  g_signal_connect(G_OBJECT(chatBtn), "clicked", G_CALLBACK(chatting), (gpointer)sendData);

  logoutBtn = gtk_button_new_with_label("Log Out!");
  gtk_widget_set_size_request(logoutBtn, 70, 30);
  gtk_table_attach(GTK_TABLE(table), logoutBtn, 3, 4, 4, 5, 
      GTK_FILL, GTK_FILL, 0, 0);

  gtk_container_add(GTK_CONTAINER(window), table);

  g_signal_connect_swapped(logoutBtn, "clicked", G_CALLBACK(gtk_widget_destroy), window);
  g_signal_connect(logoutBtn, "clicked", G_CALLBACK(gtk_main_quit), NULL); 
  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));

  gtk_widget_show_all(window);
  gtk_main();
  //g_free(friendList);
}
