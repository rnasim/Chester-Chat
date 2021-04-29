#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <assert.h>
#include "list.h"
#include "chatGUI.h"
#include "Board.h"
#include "ClientFunctions.h"

static int registerOpen = 0;

/******************/
/*      MAIN      */
/******************/


int main(int argc, char *argv[])
{
    MENUDATA *menuData = g_malloc(sizeof(MENUDATA));
    // Create Socket to connect to Server
    int sockfd;
    struct sockaddr_in ServerAddress;
    struct hostent *Server;
    char username[1000];
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
    menuData->sockfd = sockfd;
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
  
  
  gtk_init(&argc, &argv);
  CreateLoginMenu(menuData);
  g_free(menuData);
  
}

/******************/
/*   FUNCTIONS    */
/******************/


void ShowPassword( GtkWidget *checkbutton, GtkWidget *entry )
{
  gtk_entry_set_visibility (GTK_ENTRY (entry),
			    GTK_TOGGLE_BUTTON (checkbutton)->active);
}

void screen_Login(GtkWidget *widget, gpointer data){
  MENUDATA *menuData = (MENUDATA*)data;
  assert(menuData);
  const gchar *entry_Username_Text;
  const gchar *entry_PW_Text;
  char Username[MAX_LENGTH];
  char Password[MAX_LENGTH];
  int blank = 0;
  
  entry_Username_Text = gtk_entry_get_text(GTK_ENTRY (menuData->entry_Login));
  entry_PW_Text = gtk_entry_get_text(GTK_ENTRY (menuData->entry_PW));
  
  if(registerOpen == 0)
  {
    // Checks if Entries are empty
    if(*entry_Username_Text == '\0')
    {
      blank = 1;
      g_print("Username cannot be empty\n");
    }
    if(*entry_PW_Text == '\0')
    {
      blank = 1;
      g_print("Password cannot be empty\n");
    }
  
    // If entries not empty, Check Username and Password
    if(blank == 0)
    {
      strcpy(Username, entry_Username_Text);
      strcpy(Password, entry_PW_Text);
      g_print("Username: %s\n", Username);
      g_print("Password: %s\n", Password);
      
      // ERROR Checking
      if(LoginFunction(Username, Password, menuData->sockfd) == 1)
      {
        g_print("The password you entered is incorrect. Please run the program and try again.");
      }
      if(LoginFunction(Username, Password, menuData->sockfd) == 2)
      {
        g_print("The username you entered is invalid. Please run the program and try again.");
      }
      
      //Successful Login
      if(LoginFunction(Username, Password, menuData->sockfd) == 0)
      {
        g_print("Succesfully Logged In\n");
        g_print("Login Screen\n");
        g_print ("Contents of entries:\n%s\n%s\n", entry_Username_Text, entry_PW_Text);
        gtk_widget_destroy(menuData->window);
        ChatMainMenu(Username, menuData->sockfd);
      }
    }
  }
}

void screen_Register(GtkWidget *widget, gpointer data){
  if(registerOpen == 0){
    MENUDATA *menuData = (MENUDATA*)data;
    REGISTERDATA *registerData = g_malloc(sizeof(*registerData));
    
    registerOpen = 1;
    registerData->sockfd = menuData->sockfd;
    CreateRegisterMenu(registerData);
    printf("Register Screen\n");
  } 
}

void exit_Register(GtkWidget *widget, gpointer data){
  REGISTERDATA *registerData = (REGISTERDATA*)data;
  assert(registerData);
  //g_free(registerData);
  registerOpen = 0;
}

void Register_function(GtkWidget *widget, gpointer data){ //When Register Button is clicked
  REGISTERDATA *registerData = (REGISTERDATA*)data;
  assert(registerData);
  const gchar *entry_Username_Text;
  const gchar *entry_PW_Text;
  const gchar *entry_CPW_Text;
  
  char Username[MAX_LENGTH];
  char Password[MAX_LENGTH];
  int blank = 0;
  
  entry_Username_Text = gtk_entry_get_text(GTK_ENTRY (registerData->entry_Username));
  entry_PW_Text = gtk_entry_get_text(GTK_ENTRY (registerData->entry_PW));
  entry_CPW_Text = gtk_entry_get_text(GTK_ENTRY (registerData->entry_CPW));
  
  // Checks if Entries are empty
  if(*entry_Username_Text == '\0')
  {
    blank = 1;
    g_print("Username cannot be empty\n");
  }
  if(*entry_PW_Text == '\0')
  {
    blank = 1;
    g_print("Password cannot be empty\n");
  }
  if(*entry_CPW_Text == '\0')
  {
    blank = 1;
    g_print("Password cannot be empty\n");
  }
  
  // If entries not emptied, Check if Passwords are same
  if(blank == 0)
  {
    if(ComparePasswords(entry_PW_Text, entry_CPW_Text) == 0)
    {
      strcpy(Username, entry_Username_Text);
      strcpy(Password, entry_PW_Text);
      int pass = Registration(Username, Password, registerData->sockfd);
      g_print("pass = %d\n", pass);
      if(pass == 1)
      {
        g_print("The username you entered has already been taken. Please run the program and try again.");
        g_print("pass = %d\n", pass);
      }
      else if(pass == 0)
      {
        g_print("pass = %d\n", pass);
        g_print("Registered!\n");
        g_print ("Registered:\n%s\n%s\n%s\n", entry_Username_Text, entry_PW_Text, entry_CPW_Text);
        gtk_widget_destroy(registerData->window);
        g_free(registerData);
        registerOpen = 0;
      }
    }
    else
    {
      g_print ("Contents of entries:\n%s\n%s\n%s\n", entry_Username_Text, entry_PW_Text, entry_CPW_Text);
      g_print("Passwords do not match!\n");
    }
  }
  
}

int ComparePasswords(const gchar *PW, const gchar *CPW){  //Returns 0 if Passwords are same
  int compare = strcmp(PW, CPW);
  if(compare != 0){
    return 1;
  } 
  else{
    return 0;
  }
}


void CreateRegisterMenu(REGISTERDATA *registerData){

  assert(registerData);
  // Declare Widgets
  GtkWidget *window_Register;
  GtkWidget *vbox;
  GtkWidget *label_Name, *label_PW, *label_CPW;
  GtkWidget *entry_Name, *entry_PW, *entry_CPW;
  GtkWidget *button_Register;
  GtkWidget *check_ShowPW, *check_ShowCPW;
  gint tmp_pos;

  
  // Window
  window_Register = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  registerData->window = window_Register;
  gtk_container_set_border_width (GTK_CONTAINER (window_Register), 10);
  g_signal_connect(window_Register, "destroy", G_CALLBACK(exit_Register), (gpointer)registerData);
  g_signal_connect_swapped(window_Register, "delete-event", G_CALLBACK(gtk_widget_destroy), window_Register);
  gtk_window_set_position(GTK_WINDOW(window_Register), GTK_WIN_POS_CENTER) ; 
	gtk_window_set_resizable(GTK_WINDOW(window_Register), FALSE) ; 
  
  
  // VBOX
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window_Register), vbox);
  gtk_widget_show(vbox);
  
  
  // Labels
  label_Name = gtk_label_new("Username");
  gtk_widget_show(label_Name);
  label_PW = gtk_label_new("Password");
  gtk_widget_show(label_PW);
  label_CPW = gtk_label_new("Confirm Password");
  gtk_widget_show(label_CPW);
  
  
  // Username Entry
  registerData->entry_Username = gtk_entry_new();
  entry_Name = registerData->entry_Username;
  gtk_entry_set_max_length(GTK_ENTRY(entry_Name), 50);
  tmp_pos = GTK_ENTRY(entry_Name)-> text_length;
  gtk_editable_select_region (GTK_EDITABLE (entry_Name), 0, GTK_ENTRY (entry_Name)->text_length);
  gtk_widget_show(entry_Name);
  
  
  // Password Entry 
  registerData->entry_PW = gtk_entry_new();
  entry_PW = registerData->entry_PW;
  gtk_entry_set_max_length(GTK_ENTRY(entry_PW), 50);
  tmp_pos = GTK_ENTRY(entry_PW)-> text_length;
  gtk_editable_select_region (GTK_EDITABLE (entry_PW), 0, GTK_ENTRY (entry_PW)->text_length);
  gtk_entry_set_visibility (GTK_ENTRY (entry_PW), FALSE);
  gtk_widget_show(entry_PW);
  
  
  // Confirm Password Entry
  registerData->entry_CPW = gtk_entry_new();
  entry_CPW = registerData->entry_CPW;
  gtk_entry_set_max_length(GTK_ENTRY(entry_CPW), 50);
  tmp_pos = GTK_ENTRY(entry_CPW)-> text_length;
  gtk_editable_select_region (GTK_EDITABLE (entry_CPW), 0, GTK_ENTRY (entry_CPW)->text_length);
  gtk_entry_set_visibility (GTK_ENTRY (entry_CPW), FALSE);
  gtk_widget_show(entry_CPW);
 
 
  // Display Password or not 
  check_ShowPW = gtk_check_button_new_with_label("Show Password");
  g_signal_connect(check_ShowPW, "toggled", G_CALLBACK(ShowPassword), entry_PW);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_ShowPW), FALSE);
  gtk_widget_show(check_ShowPW);
  
  
  // Display Password or not 
  check_ShowCPW = gtk_check_button_new_with_label("Show Password");
  g_signal_connect(check_ShowCPW, "toggled", G_CALLBACK(ShowPassword), entry_CPW);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_ShowCPW), FALSE);
  gtk_widget_show(check_ShowCPW);
  
  
  // Register Button
  button_Register = gtk_button_new_with_label(" Register ");
  g_signal_connect(button_Register, "clicked", G_CALLBACK(Register_function), (gpointer)registerData);
  gtk_widget_show(button_Register);
  
  
  // Place Widgets in VBOX
  gtk_box_pack_start(GTK_BOX(vbox), label_Name, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), entry_Name, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), label_PW, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), entry_PW, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), check_ShowPW, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), label_CPW, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), entry_CPW, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), check_ShowCPW, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), button_Register, FALSE, FALSE, 25);
  
  gtk_widget_show(window_Register);
  
}

void CreateLoginMenu(MENUDATA *menuData){
  
  assert(menuData);
  // Declare Widgets
  GtkWidget *window_Login;
  GtkWidget *button_Login, *button_Register, *button_Exit, *check_ShowPW;
  GtkWidget *fixed_Login;
  GtkWidget *label_Login, *label_PW;
  gint tmp_pos;
  
  // Initialize Widgets //
  
  // Window
  window_Login = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  menuData->window = window_Login;
  gtk_container_set_border_width (GTK_CONTAINER (window_Login), 10);
  g_signal_connect (window_Login, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect_swapped (window_Login, "delete-event", G_CALLBACK (gtk_widget_destroy), window_Login);
  gtk_window_set_position(GTK_WINDOW(window_Login), GTK_WIN_POS_CENTER) ; 
	gtk_window_set_title(GTK_WINDOW(window_Login), "Chesster Chat") ; 
	gtk_window_set_resizable(GTK_WINDOW(window_Login), FALSE) ; 
  
  
  // Fixed Container
  fixed_Login = gtk_fixed_new();
  gtk_container_add (GTK_CONTAINER(window_Login), fixed_Login);
  gtk_widget_show(fixed_Login);
  
  
  // Labels
  label_Login = gtk_label_new("Username");
  gtk_widget_show(label_Login);
  label_PW = gtk_label_new("Password");
  gtk_widget_show(label_PW);
  
  
  // Enter Login Text 
  menuData->entry_Login = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(menuData->entry_Login), 50);
  tmp_pos = GTK_ENTRY(menuData->entry_Login)-> text_length;
  gtk_editable_select_region (GTK_EDITABLE (menuData->entry_Login), 0, GTK_ENTRY (menuData->entry_Login)->text_length);
  gtk_widget_show(menuData->entry_Login);
  
  
  // Enter Password Text
  menuData->entry_PW = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(menuData->entry_PW), 50);
  tmp_pos = GTK_ENTRY(menuData->entry_PW)-> text_length;
  gtk_editable_select_region (GTK_EDITABLE (menuData->entry_PW), 0, GTK_ENTRY (menuData->entry_PW)->text_length);
  gtk_entry_set_visibility (GTK_ENTRY (menuData->entry_PW), FALSE);
  gtk_widget_show(menuData->entry_PW);
  
  
  // Display Password or not 
  check_ShowPW = gtk_check_button_new_with_label("Show Password");
  g_signal_connect(check_ShowPW, "toggled", G_CALLBACK(ShowPassword), menuData->entry_PW);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_ShowPW), FALSE);
  gtk_widget_show(check_ShowPW);
    
    
  // Exit Button
  button_Exit = gtk_button_new_with_label (" Exit ");
  g_signal_connect_swapped(button_Exit, "clicked", G_CALLBACK (gtk_widget_destroy), window_Login);
  g_signal_connect(button_Exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show (button_Exit);
  
  
  // Login Button
  button_Login = gtk_button_new_with_label ("   Login   ");
  g_signal_connect(button_Login, "clicked", G_CALLBACK(screen_Login), (gpointer)menuData);
  gtk_widget_show(button_Login);
  
  
  // Register Button
  button_Register = gtk_button_new_with_label ("  Register  ");
  g_signal_connect(button_Register, "clicked", G_CALLBACK(screen_Register), (gpointer)menuData);
  gtk_widget_show(button_Register);
  
  
  // Place widgets on Fixed Container 
  gtk_fixed_put(GTK_FIXED(fixed_Login), label_Login, 0, 25);
  gtk_fixed_put(GTK_FIXED(fixed_Login), label_PW, 0, 50);
  
  gtk_fixed_put(GTK_FIXED(fixed_Login), menuData->entry_Login, 75, 20);
  gtk_fixed_put(GTK_FIXED(fixed_Login), menuData->entry_PW, 75, 45);
  
  gtk_fixed_put(GTK_FIXED(fixed_Login), button_Login, 10, 95);
  gtk_fixed_put(GTK_FIXED(fixed_Login), button_Register, 140, 95);
  
  gtk_fixed_put(GTK_FIXED(fixed_Login), check_ShowPW, 75, 70);
  gtk_fixed_put(GTK_FIXED(fixed_Login), button_Exit, 85, 125);
  gtk_widget_show (window_Login);
  
  gtk_main();

}
