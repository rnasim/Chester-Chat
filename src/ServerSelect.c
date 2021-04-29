nclude <stdio.h>  
#include <string.h>    
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>    
#include <arpa/inet.h>    
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> 
 
     
int main(int argc , char *argv[])   
{   
    int opt = 1;   
    int master_socket, addrlen, new_socket, max_clients = 30, activity, i, valread, sd;  
	int client_socket[500]= 0;
    int max_sd;   
    struct sockaddr_in address;   
         
    char buffer[1025];  
	int PORT = atoi(argv[1]);
         
    /*set of socket descriptors*/
	/*fd_set: contains list of file descriptors to moitor for some activity*/
    /*readfds will monitor all the active file descriptosr of the clients 
 * 	plus that of the main server listening socket*/
	fd_set readfds;   
         
    /*message*/
    char *message = "ECHO Chesster Chat v1.0 \r\n"; 
         
    /*create a master socket*/
	master_socket = socket(AF_INET , SOCK_STREAM , 0);
    if(master_socket == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }    
     
    /*essential for creating socket*/
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT );   
         
    /*bind the socket to localhost port*/ 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT);   
         
    /*specify maximum of 3 pending connections for the master socket*/
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
         
    /*accept the incoming connection*/
    addrlen = sizeof(address);   
    puts("Waiting for connections ...");   
         
    while(1)   
    {   
        /*FD_ZERO clears an FD set, clear the socket set*/
        FD_ZERO(&readfds);   
     
        /*FD_SET adds a descriptor to a FD_SET, add master socket to set*/
        /*Whenever a new client connects, master_socket will activate and a new FD will be open
 * 		for that client. We will store its FD in out client_list and in the next iteration
 * 				add it to the readfds to monitor for activity from this client. If an old client sends
 * 						data, readfds will be activated and we will check from the list of existing clients to see
 * 								which client has sent the data*/
		FD_SET(master_socket, &readfds); /*server socket is active, takes that fd set and puts it in that fd set*/
        max_sd = master_socket;   
             
        /*add child sockets to set*/
        for (i = 0 ; i < max_clients ; i++)   
        {   
            /*socket descriptor*/ 
            sd = client_socket[i];   
                 
            /*if valid socket descriptor then add to read list*/  
            if(sd > 0) {  
                FD_SET(sd , &readfds);   
			}   
            /*highest file descriptor number, need it for the select function*/ 
            if(sd > max_sd) { 
                max_sd = sd;  
			}				
        }   
      
		/*select checks to see if any sockets are ready for reading, writing or exceptional condition pending
 * 		first parameter is the maximum number of sockets to check, the second parameter is to check if any socket is 
 * 				ready for reading*/
		/*looks in list of active file descriptors (fd's) */
        activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
             
		/*FD_ISSET: if something happened on the master socket, then it's an incoming connection*/
		/*will accept*/
        if (FD_ISSET(master_socket, &readfds))   
        {   
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            /*inform user of socket number - used in send and receive commands*/  
            printf("New connection , socket fd is %d , ip is : %s , port : %d  
                  \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs 
                  (address.sin_port));   
           
            /*send new connection greeting message*/
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )   
            {   
                perror("send");   
            }   
                 
            puts("Welcome message sent successfully");   
                 
            /*add new socket to array of sockets*/
            for (i = 0; i < max_clients; i++)   
            {   
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                         
                    break;   
                }   
            }   
        }   
             
        /*else its some IO operation on some other socket*/
        for (i = 0; i < max_clients; i++)   
        {   
            sd = client_socket[i];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                /*Check if it was for closing , and also read the  
 *                 incoming message*/
                if ((valread = read( sd , buffer, 1024)) == 0)   
                {   
                    /*Somebody disconnected , get his details and print*/
                    getpeername(sd , (struct sockaddr*)&address , \ 
                        (socklen_t*)&addrlen);   
                    printf("Host disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
                         
                    /*Close the socket and mark as 0 in list for reuse*/ 
                    close( sd );   
                    client_socket[i] = 0;   
                }   
                     
                /*Echo back the message that came in*/  
                else 
                {   
                    /*set the string terminating NULL byte on the end  
 *                     of the data read*/
                    buffer[valread] = '\0';   
                    send(sd , buffer , strlen(buffer) , 0 );   
                }   
            }   
        }   
    }   
         
    return 0;   
}   
