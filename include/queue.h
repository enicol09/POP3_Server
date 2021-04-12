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