#include "queue.h"

extern request *front = NULL;
extern request *rear = NULL;

void enqueue(int *client, struct sockaddr_in *client_ptr) {
    request *r = malloc(sizeof(request));
    r->socket = client;
    r->client_ptr = client_ptr;
    r->new = NULL;
    if (rear == NULL) {
        front = r;
    } else {
        rear->new = r;
    }
    rear = r;
}

int dequeue(request *r) {
    if (front==NULL) {
        return -1;
    } else {
        r->socket = front->socket;
        r->client_ptr = front->client_ptr;
        request *temp = front;
        front = front->new;
        if (front==NULL)
        {
            rear = NULL;
        }
        // free(temp);
        return 1;
    }
}
