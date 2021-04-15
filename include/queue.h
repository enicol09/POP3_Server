/** @file queue.h
 *  @brief This is a h file that helps the implementation of Hw_4.
 *         It contains all the function declarations as they are implemented at the c files.
 *  
 *  You can see below all the function/libraries declarations.
 *
 *  @author Elia Nicolaou, Pantelis Mikelli, Michail-Panagiotis Mpofos
 *  @version 1
 *  @bug No know bugs.
 *  @see server.c, utilities.c , queue.c
 */
#pragma once
#include <pthread.h>
#include <sys/socket.h> // For sockets

typedef struct request{
    struct request *new;
    struct sockaddr_in *client_ptr;
    int *socket;
} request;

void enqueue(int *client, struct sockaddr_in *client_ptr);

int dequeue(request *r);