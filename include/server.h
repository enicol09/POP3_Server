/** @file server.h
 *  @brief This is a h file that helps the implementation of Hw_4.
 *         It contains all the function declarations as they are implemented at the c files.
 *  
 *  You can see below all the function/libraries declarations.
 *
 *  @author Elia Nicolaou, Pantelis Mikelli, Michail-Panagiotis Mpofos
 *  @version 1
 *  @bug No know bugs.
 *  @see server.c, utilities.c
 */

#pragma once
#include <sys/types.h> // For sockets
#include <sys/socket.h> // For sockets
#include <netinet/in.h> // For Internet sockets
#include <netdb.h> // For gethostbyaddr
#include <pthread.h>
#include "library.h"
#include "queue.h"

#define RESPONSE_LENGTH 512
#define OKAY "+OK "
#define ERROR "-ERR "

extern char *command[] = {
    "USER",
    "PASS",
    "STAT",
    "LIST",
    "RETR",
    "DELE",
    "QUIT",
    //bonus section:
    "RSET",
    "NOOP",
    "MAKE",
    "SEND",
    "HELP"
};

void *manage_thread(void *arg);
void manage_request(int *client, struct sockaddr_in *client2);
void sig_handler(int signum,int sock);
void toUpper(char *input);
int getLength();
bool checkFormat(char *input);
int decodeInput(char *input);
