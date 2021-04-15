#include "queue.h"

extern request *front = NULL;
extern request *rear = NULL;

 /**
 * @brief This function pushes a job in the queue.
 *
 * We call enqueue in order to push a job in the tail of the queue.
 *
 * @param int *client file descriptor of each client
 * @param struct sockaddr_in *client_ptr socket of each client
 * @return void
 * */
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

  /**
 * @brief This function unqueues a job from the queue.
 *
 * We call edequeue in order to pull a job from the head of the queue.(And update the new head to the next job)
 *
 * @param request *r pointer to return socket and file descriptor
 * @return int -1 if unqueue failed
 * */
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
