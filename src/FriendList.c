/*FriendList:
 1. A client can request a friend.
 -by doing special word REQUEST <username>
 
 2. That friend will be able to see their friend requests.
 Can make a friend request list.
 That friend will accept or deny the friend request.
 3. Each friend list will be stored in a unique text file.
 If they accept, need to update both friend's lists (text files).
 - use a function that will add to the text file
 -to update on GUI, read through and print text file.
 If they deny, need to delete the friend request.
 -if possible, if they deny then can go through the friend
 request list text file and replace the username with " "
 1. Once a user has registered, we need to create a friend list for them.
 2. If they do REQUEST <username> we have to call AddToRequestList for the user they're requesting
 3. If the user views their request list by doing VIEW <list> and does ACCEPT <username> call AddFriend
 4. If the user views their request by doing VIEW <list> and does DENY <username> call RemoveFromRequestList
 5. If the user views their friend list by doing VIEW <list> and does REMOVE <username> call RemoveFriend for both users.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*Create a friends list text file for a certain username*/
void CreateFriendListFile(char *username){
    char* extension = "friendlist.txt";
    char fileSpec[strlen(username)+strlen(extension)+1];
    
    snprintf( fileSpec, sizeof(fileSpec), "%s%s", username, extension );
    
    FILE *userfriendlist = fopen(fileSpec , "a");
    fprintf(userfriendlist, "%s's Friend List: \n", username);
}

void CreateRequestListFile(char *username){
    
    char* extension = "requestlist.txt";
    char fileSpec[strlen(username)+strlen(extension)+1];
    
    snprintf( fileSpec, sizeof(fileSpec), "%s%s", username, extension );
    
    FILE *userrequestlist = fopen(fileSpec, "a");
    fprintf(userrequestlist, "%s's Request List: \n", username);
}

/*Will print the friend list or request list to the client's window*/
void ViewList(char *username, char *list){
    /*if (fptr == NULL)
     {
     printf("Cannot open file \n");
     exit(0);
     } */
    
    /*Read contents from file*/
    FILE *l = fopen(list, "r");
    char c = fgetc(l);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(l);
    }
    
}
char *GetFriendListName(char *username){
    char *filename = malloc(sizeof(char) *1000);
    char *extension = "friendlist.txt";
    strcpy(filename, username);
    strcat(filename, extension);
    return filename;
}
char *GetRequestListName(char *username){
    
    char *filename = NULL;
    char *extension = "requestlist.txt";
    strcpy(filename, username);
    strcat(filename, extension);
    return filename;
}

int AddToRequestList(char *requestusername, char *acceptusername, char *requestlist){
    char string[1000], newstring[1000];
	FILE *userlist = fopen("UsernamesPasswords.txt", "r");
	
	while(fgets(string, sizeof(string), userlist) != NULL){
		strtok(string, "\n");
		while(fgets(newstring, sizeof(newstring), requestlist) != NULL) {
			if(strcmp(newstring, requestusername) == 0) {
				printf("You already requested this person!\n");
				return 1;
			}
		}
	}
	strtok(friendusername, "\n");
    fprintf(requestlist, "%s\n", requestusername);
	fclose(requestlist);
    return 0;
}

int RemoveFromRequestList(char *removeusername, char *username, char *requestlist){
    char *friends[100];
    char string[1000] = NULL;
    int numfriends = 0;
	for(numfriends = 0; fgets(string, sizeof(string), friendlist) != NULL;, numfriends++){
		strtok(string, "\n");
		friends[numfriends] = (char*)malloc(sizeof(string));
		strcpy(friends[numfriends], string);
	}

    for(int i = 0; i < numfriends; i++)
    {
        if(strcmp(friends[i], removeusername) == 0)
        {
            friends[i] = "#DELFRIEND#";
            break;
        }
        if(i == numfriends-1 && strcmp(friends[i], removeusername) != 0)
        {
			printf("Error removing friend. Please try again. \n");
            return 1;
        }
    }
	char *filename = GetRequestListName(username);
	remove(filename);
	FILE *newfile = fopen(filename, "a");
    for(int i = 0; i < numfriends; i++)
    {
        if(strcmp(friends[i], "#DELFRIEND#") != 0)
        {
            fprintf(newfile, "%s\n", friends[i]);
        }
    }
	fclose(newfile);
    return 0;
}
/*Add a friend to a user's friendlist text file*/
int AddFriend(char *friendusername, char *yourusername, FILE *friendsfile, FILE *yourfile)
{
    /*append the requestusername (the user who is requesting to be their friend if they choose to accept*/ //friend
    char string[1000], newstring[1000];
    char *friendsfilename = GetFriendListName(friendusername);
    FILE *userlist = fopen("UsernamesPasswords.txt", "r");
    while(fgets(string, sizeof(string), userlist) != NULL)
    {
        strtok(string, "\n");
        if(strcmp(friendusername, string) == 0)
        {
            strcat(friendusername, "\n");
            while(fgets(newstring, sizeof(newstring), yourfile) != NULL)
            {
                if(strcmp(newstring, friendusername) == 0)
                {
                    printf("This person is already your friend! \n");
                    return 1;
                }
            }
            strtok(friendusername, "\n");
            fprintf(friendsfile, "%s\n", yourusername);
            fprintf(yourfile, "%s\n", friendusername);
            fclose(yourfile);
            fclose(friendsfile);
            fclose(userlist);
            return 0;
        }
    }
    printf("Invalid username. Please try again. \n");
    remove(friendsfilename);
    return 1;
}

/*Remove a friend from a user's friendlist text file*/
int RemoveFriend(char *removeusername, char *username, FILE *friendlist){
    char *friends[100];
    char string[1000];
    int numfriends = 0;
    for(numfriends = 0; fgets(string, sizeof(string), friendlist) != NULL; numfriends++)
    {
        strtok(string, "\n");
        friends[numfriends] = (char*)malloc(sizeof(string));
        strcpy(friends[numfriends], string);
    }
    for(int i = 0; i < numfriends; i++)
    {
        if(strcmp(friends[i], removeusername) == 0)
        {
            friends[i] = "#DELFRIEND#";
            break;
        }
        if(i == numfriends-1 && strcmp(friends[i], removeusername) != 0)
        {
            printf("Error removing friend. Please try again. \n");
            return 1;
        }
    }
    char *filename = GetFriendListName(username);
    remove(filename);
    FILE *newfile = fopen(filename, "a");
    for(int i = 0; i < numfriends; i++)
    {
        if(strcmp(friends[i], "#DELFRIEND#") != 0)
        {
            fprintf(newfile, "%s\n", friends[i]);
        }
    }
    fclose(newfile);
    return 0;
}