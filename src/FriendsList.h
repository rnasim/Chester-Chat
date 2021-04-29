#ifndef FriendsList_h
#define FriendsList_h

void CreateFriendListFile(char username[1000]);
void CreateRequestListFile(char username[1000]);
void ViewList(char *username, char *list);
char *GetFriendListName(char *username);
char *GetRequestListName(char *username);
int AddToRequestList(char *requestusername, char *acceptusername, char *requestlist);
int RemoveFromRequestList(char *removeusername, char *username, char *requestlist);
int AddFriend(char *friendusername, char *yourusername, FILE *friendsfile, FILE *yourfile);
int RemoveFriend(char *removeusername, char *username, FILE *friendlist);

#endif /* FriendsList_h */
