/* File: int_str_server.c */
#include "library.h"
#include <sys/types.h> // For sockets
#include <sys/socket.h> // For sockets
#include <netinet/in.h> // For Internet sockets
#include <netdb.h> // For gethostbyaddr
#include <stdio.h> // For I/O
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h> 
#include <stdbool.h>

#define RESPONSE_LENGTH 512
#define OKAY "+OK "
#define ERROR "-ERR "

char *command[] = {
    "USER",
    "PASS",
    "STAT",
    "LIST",
    "RETR",
    "DELE",
    "QUIT"
};
bool argument[]={
    true,
    true,
    false,
    true,
    true,
    true,
    false
};
void toUpper(char *input){
   int i;
   for (i = 0; input[i]!='\0'; i++) {
      if(input[i] >= 'a' && input[i] <= 'z') {
         input[i] = input[i] -32;
      }
   }
}
int getLength(){
    return sizeof(command)/sizeof(char*);
}
bool checkFormat(char *input){
    int last = strlen(input) -1;
    return input[last] == '\n' && input[last-1]=='\r';
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

int main(int argc, char *argv[]) /* Server with Internet stream sockets */
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
       write(newsock,OKAY,4);
       write(newsock,"POP3 server ready\r\n",19);
     //reverse DNS
     rem = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof client.sin_addr.s_addr, /* Find client's address */client.sin_family);
     if (rem == NULL)
        {
         perror("gethostbyaddr");
         exit(1);
        }

      printf("Accepted connection from %s\n", rem -> h_name);

      //display(newsock);
    bool nextLoop = true;
    bool transaction = false;
    bool userIn = false;
    char *username = NULL;
    bool *emails = NULL;
    int mail;
    int *names = NULL;
    int plithos,megethos,many = -1;
                do {
                   bzero(input, sizeof input); /* Initialize buffer */
                   if (read(newsock, input, sizeof input) < 0)
                      { /* Receive message */
                       perror("read");
                       exit(1);
                      }
                   printf("Read string: %s\n", input);
                   int code = decodeInput(input);
                   printf("eheihreirhere0\r\n");
                       if(code == 0){
                            write(newsock,"USER\r\n",6);
                            int size = strlen(input) - 4;
                            char user[size];
                            int position = 5;`
                            int c = 0;
                            int length = size;
                            while ( c < length) {
                               user[c] = input[position];
                               c++;
                               position++;
                            }
                            user[c] = '\0';
                            if(userExists(user) == false){
                                chat *stupiderror ="-ERR this is stupid, i have never in my life heard of mailbox";
                                write(newsock,"-ERR this is stupid, i have never in my life heard of mailbox")
                            }
                            userIn = true;
                             //username = "mbofos01";
                            many = howMany(user);
                            emails = malloc(sizeof(bool)*many);
                            printf("Mails: %d\r\n",many);
                            int i =0;
                            for(i=0;i<many;i++)
                                emails[i] = true;
                            names = malloc(sizeof(int)*many);
                            fillNames(user,names);
                        }else if(code ==1 ){
                            write(newsock,"PASS\r\n",6);
                            char *password = NULL;
                            if(userIn && !transaction) {
                                if(passwordCheck(username,password)){
                                    transaction = true;
                                }
                                  else{
                                    nextLoop = false;
                                }      
                            }              
                        } else if (code == 2){
                            write(newsock,"STATS\r\n",6);
                            if(transaction){
                                activeStats(username,emails,&plithos,&megethos);
                                //printf("How many: %d and size %d\n",plithos,megethos);
                            }
                        }
                        else if (code ==3){

                            write(newsock,"LIST\r\n",6);
                            if(transaction){
                                list(username,mail,emails,names,many);
                                listEmpty(username,emails,names,many);
                            }
                        }                       
                        else if(code == 4){
                            write(newsock,"RETR\r\n",6);
                            if(transaction){
                                retrieveMail(mail,username);
                            }
                        }                      
                        else if (code == 5 ) {
                            write(newsock,"DELE\r\n",6);
                            if(transaction){
                                deleteMail(mail,emails,many,names);
                            }  
                              }
                                   
                        else if (code == 6 ) {
                            write(newsock,"QUIT\r\n",6);
                            if(transaction){
                                update(username,emails,names,many);
                                nextLoop = false;
                            }
                        }
                        else {
                            write(newsock,ERROR,4);
                            write(newsock,"\r\n",2);
                        }
                   
                   bzero(input, sizeof input);


                  } while (nextLoop); /* Finish on "end" */
                        free(emails);
                        free(names);
         close(newsock); /* Close socket */
         printf("Connection from %s is closed\n", rem -> h_name);
         //exit(0);
        }

        return 0;
    }

