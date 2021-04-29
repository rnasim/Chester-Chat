#ifndef CHATGUI_H
#define CHATGUI_H

typedef struct menuData MENUDATA;
typedef struct registerData REGISTERDATA;

struct menuData
{
	GtkWidget *window;
	GtkWidget *entry_Login;
	GtkWidget *entry_PW;
  int sockfd;
};

struct registerData
{
  GtkWidget *window;
  GtkWidget *entry_Username;
  GtkWidget *entry_PW;
  GtkWidget *entry_CPW;
  int sockfd;
};


void CreateRegisterMenu(REGISTERDATA *registerData);
void CreateLoginMenu(MENUDATA *menuData);
int ComparePasswords(const gchar *PW, const gchar *CPW);
void Register_function(GtkWidget *widget, gpointer data);
void screen_Register(GtkWidget *widget, gpointer data);
void exit_Register(GtkWidget *widget, gpointer data);
void screen_Login(GtkWidget *widget, gpointer data);
void ShowPassword( GtkWidget *checkbutton, GtkWidget *entry );

#endif
