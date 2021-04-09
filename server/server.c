/* File: int_str_server.c */
#include <sys/types.h> // For sockets
#include <sys/socket.h> // For sockets
#include <netinet/in.h> // For Internet sockets
#include <netdb.h> // For gethostbyaddr
#include <stdio.h> // For I/O
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <stdbool.h>

char *command[] = {
    "USER",
    "PASS",
    "STAT",
    "LIST",
    "RETR",
    "DELE",
    "QUIT"
}
bool *argument[]={
    true,
    true,
    false,
    true,
    true,
    true,
    false
}
void toUpper(char *input){
   int i;
   for (i = 0; input[i]!='\0'; i++) {
      if(input[i] >= 'a' && input[i] <= 'z') {
         input[i] = input[i] -32;
      }
   }
}
main(int argc, char *argv[]) /* Server with Internet stream sockets */
{
 int port, sock, newsock, serverlen;//, clientlen;
 socklen_t clientlen;
 char input[256];
 struct sockaddr_in server, client;
 struct sockaddr *serverptr, *clientptr;
 struct hostent *rem;

 
 if (argc < 2)
    { /* Check if server's port number is given */
     printf("Please give the port number\n");
     exit(1);
    }
 /* The AF_INET address family is the address family for IPv4, AF_INET6 for IPv6, AF_BLUETOOTH for Bluetooth */
 /* IPPROTO_TCP is chosen for the TCP protocol if the type was set to SOCK_STREAM */
 /* IPPROTO_UDP is chosen for the UDP protocol if the type was set to SOCK_DGRAM */
 if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    { /* Create socket */
     perror("socket");
     exit(1);
    }

 port = atoi(argv[1]); /* Convert port number to integer */
 server.sin_family = AF_INET; /* Internet domain */
 /* When INADDR_ANY (0.0.0.0) is specified, the socket will be bound to all local interfaces */
 /* INADDR_LOOPBACK (127.0.0.1) always refers to the local host via the loopback device */
 server.sin_addr.s_addr = htonl(INADDR_ANY); /* My Internet address */
 server.sin_port = htons(port); /* The given port */
 serverptr = (struct sockaddr *) &server;
 serverlen = sizeof server;

 if (bind(sock, serverptr, serverlen) < 0)
    { /* Bind socket to an address */
     perror("bind");
     exit(1);
    }
 /* Waiting queue for incoming TCP connections (second param is capacity) */
 if (listen(sock, 5) < 0)
    { /* Listen for connections */
     perror("listen");
     exit(1);
    }

 printf("Listening for connections to port %d\n", port);
 
 while(1)
    {
     clientptr = (struct sockaddr *) &client;
     clientlen = sizeof client;
     if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
        {
         perror("accept");
         exit(1);
        } /* Accept connection */

     //reverse DNS
     rem = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof client.sin_addr.s_addr, /* Find client's address */client.sin_family);
     if (rem == NULL)
        {
         perror("gethostbyaddr");
         exit(1);
        }

      printf("Accepted connection from %s\n", rem -> h_name);
      display(newsock);
      bool nextLoop = true;
                do {
                   bzero(input, sizeof input); /* Initialize buffer */
                   if (read(newsock, input, sizeof input) < 0)
                      { /* Receive message */
                       perror("read");
                       exit(1);
                      }
                   printf("Read string: %s\n", input);
                   int code = decodeInput(input);
                   switch(code){
                       case 0:
                            printf("USER\n");
                            break;
                        case 1:
                            printf("PASS\n");
                            break;                       
                        case 2:
                            printf("STATS\n");
                            break;                       
                        case 3:
                            printf("LIST\n");
                            break;                       
                        case 4:
                            printf("RETR\n");
                            break;                       
                        case 5:
                            printf("DELE\n");
                            break;                       
                        case 6:
                            printf("QUIT\n");
                            break;
                        default:
                            write(newsock,"-ERR\r\n",6);
                   }
                   bzero(input, sizeof input);


                  } while (nextLoop); /* Finish on "end" */
                
         close(newsock); /* Close socket */
         printf("Connection from %s is closed\n", rem -> h_name);
         exit(0);
        }
    }
}
int getLength(){
    return sizeof(command)/sizeof(char*);
}
int decodeInput(char *input){
    //toUpper(input);
    int i = 0, function = -1;
    char code[5];
    for(i=0;i<4;i++){
        code[i] = input[i];
    }
    code[4] = '\0';
    toUpper(code);
    for(i=0;i<getLength();i++){
        if(strcmp(command[i],code)==0){
            function = i;
        }
    }
    return function;
}
