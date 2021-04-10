#include <sys/types.h> /* For sockets */
#include <sys/socket.h> /* For sockets */
#include <netinet/in.h> /* For Internet sockets */
#include <netdb.h> /* For gethostbyname */
#include <stdio.h> /* For I/O */
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

main(int argc, char *argv[]) /* Client with Internet stream sockets */
{
 int port, sock, serverlen; char buf[256];
 struct sockaddr_in server;
 struct sockaddr *serverptr;
 struct hostent *rem;
 int i;

 if (argc < 3)
    { /* Check if server's host name and port number are given */
     printf("Please give the host name and the port number\n");
     exit(1);
    }

 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { /* Create socket */
     perror("socket");
     exit(1);
    }

 rem = gethostbyname(argv[1]);
 if (rem == NULL)
    { /* Find server's address */
     perror("gethostbyname");
     exit(1);
    }

 port = atoi(argv[2]); /* Convert port number to integer */
 server.sin_family = AF_INET; /* Internet domain */
 bcopy((char *) rem -> h_addr, (char *) &server.sin_addr, rem -> h_length);
 server.sin_port = htons(port); /* Server's Internet address and port */
 serverptr = (struct sockaddr *) &server;
 serverlen = sizeof server;

 if (connect(sock, serverptr, serverlen) < 0)
    { /* Request connection */
     perror("connect");
     exit(1);
    }

 printf("Requested connection to host %s port %d\n", argv[1], port);

 printf("\n");

 // for (i=0; i<40; i++)
 //     {
 //      bzero(buf, sizeof buf);
 //      read(sock, buf, sizeof buf);
 //      printf("%s", buf);
 //      write(sock, "ok", sizeof (char *));
 //     }
 //
 // printf("\n");
//read the accept
bzero(buf, sizeof buf); /* Initialize buffer */
if (read(sock, buf, sizeof buf) < 0)
   { /* Receive message */
    perror("read");
    exit(1);
   }
    printf("S: %s", buf);

    bzero(buf, sizeof buf); /* Initialize buffer */
    if (read(sock, buf, sizeof buf) < 0)
       { /* Receive message */
        perror("read");
        exit(1);
       }
        printf("%s\n", buf);

 printf("The connection terminates with the string end.\n");
 printf("Enter Commands: .\n");
 do {
    bzero(buf, sizeof buf); /* Initialize buffer */
    printf("C: ");
    gets(buf); /* Read message from stdin */

    if (write(sock, buf, sizeof buf) < 0)
       { /* Send message */
        perror("write");
        exit(1);
       }

    bzero(buf, sizeof buf); /* Initialize buffer */
    if (read(sock, buf, sizeof buf) < 0)
       { /* Receive message */
        perror("read");
        exit(1);
       }
           printf("S: %s\n", buf);
    } while (strcmp(buf, "end") != 0); /* Finish on "end" */

 close(sock); /* Close socket */
 exit(0);
}
