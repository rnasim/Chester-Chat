#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#define MAX_LENGTH 1000

void FatalError(const char *errormessage);
//void *receive(void *sockfd);
int LoginFunction(char username[MAX_LENGTH], char password[MAX_LENGTH], int sockfd);
int Registration(char username[MAX_LENGTH], char password[MAX_LENGTH], int sockfd);
int MessageClient(char username[MAX_LENGTH], char message[MAX_LENGTH], int sockfd);


#endif
