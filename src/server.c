/* File: int_str_server.c */
#include "server.h"

pthread_t* tpool;  //thread pool
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

int thread_number;  //Total threads in pool
int alive_threads;  //Number of threads that are active
int sock;  //Global so it can be used in sig_handler

void *manage_thread(void *arg) {
    //Threads run forever
    while (true) {
        int result = -1;
        request r;
        //Lock threads in order to get a request from queue
        pthread_mutex_lock(&mutex);
        if (dequeue(&r) == -1) {
            //Wait until a request is pushed in queue
            //When queue has a request one of the the threads that are inactive gets the job
            pthread_cond_wait(&cond_var, &mutex);
            result = dequeue(&r);
        }
        pthread_mutex_unlock(&mutex);
        if (result != -1) {
            //If thread gets a job manage the request
            manage_request(r.socket, r.client_ptr);

            pthread_mutex_lock(&mutex);
            alive_threads--;
            pthread_mutex_unlock(&mutex);
        }
    }
}

void manage_request(int *client, struct sockaddr_in *client2) {
    char input[256];
    struct hostent *rem;
    struct sockaddr_in client3 = *client2;
    write(*client,OKAY,4);
    write(*client,"POP3 server ready\r\n",19);
    //reverse DNS
    rem = gethostbyaddr((char *) &client3.sin_addr.s_addr, sizeof client3.sin_addr.s_addr, /* Find client's address */client3.sin_family);
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
    int mail = -1;
    int *names = NULL;
    int plithos,megethos,many = -1;
    //signal(SIGALRM,sig_handler); // Register signal handler

   
  
    do {
       
    fd_set active_fd_set;
    FD_ZERO (&active_fd_set);
    FD_SET (*client, &active_fd_set);
    
    struct timeval timeout;      
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
  
    int result = select(*client +1, &active_fd_set, NULL, NULL, &timeout);
      
       if(result<=0) {
          write(*client,"-ERR connection closed due to inactivity \r\n",strlen("-ERR connection closed due to inactivity \r\n"));
          nextLoop= false;
      }
      
      else {
      
       bzero(input, sizeof input); /* Initialize buffer */
       // alarm(60);  
        if (read(*client, input, sizeof input) < 0)
        { /* Receive message */
            perror("read");
            exit(1);
        }
               
        //alarm(60);  // Scheduled alarm after 2 seconds
        int code = decodeInput(input);
            if(code == 0){
                if(userIn)
                    write(*client,"You have already connected \r\n",30);
                else {
                int size = strlen(input) - 4;
                char user[size];
                int position = 5;
                int c = 0;
                int length = size;
                while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r'){
                    user[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                user[c] = '\0';
                

                username = malloc(sizeof(char)*c);
                strcpy(username,user);
                if(userExists(user) == false){
                    char stupidError[256];
                    bzero(stupidError,256);
                    strcat(stupidError,"-ERR never heard of mailbox ");
                    strcat(stupidError,user);
                    strcat(stupidError,"\r\n\0");
                    write(*client,stupidError,strlen(stupidError));
                    nextLoop = false;
                    close(*client);
                }
                else{
                    char stupidMsg[256];
                    bzero(stupidMsg,256);
                    strcat(stupidMsg,"+OK ");
                    strcat(stupidMsg,user);
                    strcat(stupidMsg," is a valid mailbox\r\n\0");
                    printf("%s\r\n",stupidMsg); 
                    write(*client,stupidMsg,strlen(stupidMsg));
                    //bzero(stupidMsg, sizeof stupidMsg);
                }
                    
                userIn = true;
                many = howMany(user);
                emails = malloc(sizeof(bool)*many);
                printf("Mails: %d\r\n",many);
                int i =0;
                for(i=0;i<many;i++)
                    emails[i] = true;
                names = malloc(sizeof(int)*many);
                fillNames(user,names);
                }
            }else if(code ==1 ){
                
                if(userIn){
                
                int size = strlen(input) - 4;
                char passw[size];
                int position = 5;
                int c = 0;
                int length = size;
                while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r' && input[position]!=' '){
                    passw[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                passw[c] = '\0';
                
                if(c==0){
                    write(*client,"-ERR invalid password - you need to enter a password first \r\n",strlen("-ERR invalid password - you need to enter a password first \r\n"));
                } else {
                //write(newsock,"PASS\r\n",6);
                //char *password = NULL;
                if(userIn && !transaction) {
                    if(passwordCheck(username,passw)){
                        transaction = true;
                        write(*client,"+OK maildrop locked and ready\r\n",strlen("+OK maildrop locked and ready\r\n"));
                    }
                        else{
                        write(*client,"-ERR invalid password\r\n",strlen("-ERR invalid password\r\n"));
                        nextLoop = false;

                    }
                }
                }
                }
                else{
                write(*client,"-ERR You have to insert the username first - please insert USER command :( \r\n",strlen("-ERR You have to insert the username first - please insert USER command :( \r\n")); }
            } else if (code == 2){
                
                if(transaction){
                    activeStats(username,emails,&plithos,&megethos);
                    char stat[256];
                    bzero(stat,256);
                    char lol[1000];
                    bzero(lol,1000);
                    strcat(stat,"+OK ");
                    itoa(plithos,lol);
                    strcat(stat,lol);
                    strcat(stat," ");
                    char lol2[1000];
                    bzero(lol2,1000);
                    itoa(megethos,lol2);
                    strcat(stat,lol2);
                    strcat(stat,"\r\n");
                    write(*client,stat,strlen(stat)); 
                }
            }
            else if (code ==3){
                int size = strlen(input) - 4;
                char listMe[size];
                bzero(listMe,size);
                int position = 5;
                int c = 0;
                int length = size;
                while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r' && input[position]!=' '){
                    listMe[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                listMe[c] = '\0';
                if(transaction){
                    if(c==0){
                        listEmpty(username,emails,names,many,*client);
                    }
                    else{
                        list(username,atoi(listMe),emails,names,many,*client,true);
                    } 
                    }
                }
            else if(code == 4){
                int size = strlen(input) - 4;
                char retrMe[size];
                bzero(retrMe,size);
                int position = 5;
                int c = 0;
                int length = size;
                while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r' && input[position]!=' '){
                    retrMe[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                retrMe[c] = '\0';
                //                            write(newsock,"RETR\r\n",6);
                if(transaction && c!=0){
                    retrieveMail(atoi(retrMe),username,emails,names,many,*client);
                }
                else{
                    write(*client,"-ERR\r\n",6);                    
                }
            }
            else if (code == 5 ) {
                int size = strlen(input) - 4;
                char toDel[size];
                int position = 5;
                int c = 0;
                int length = size;
                while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r' && input[position]!=' '){
                    toDel[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                toDel[c] = '\0';
                
                mail = atoi(toDel);
                if(transaction){
                    if(deleteMail(mail,emails,many,names)){
                        char print[100];
                        bzero(print,100);
                        strcat(print,"+OK message ");
                        strcat(print, toDel);
                        strcat(print, " deleted\r\n");
                        write(*client, print ,strlen(print)); 
                    }
                    else {
                        write(*client,"-ERR no such message :(\r\n" ,strlen("-ERR no such message :(\r\n" )); 
                    }
                    
                }
            }
            else if (code == 6 ) {
                if(transaction){
                    bool okay = update(username,emails,names,many);
                    if(!okay){
                        write(*client, "-ERR some deleted messages not removed\r\n" ,strlen("-ERR some deleted messages not removed\r\n")); 
                    }
                    else{
                        int left = howMany(username);
                        //printf("hello %d\r\n",left);
                        if(left == 0){
                            char print[256];
                            bzero(print,256);
                            strcat(print,"+OK ");
                            strcat(print, username);
                            strcat(print, " POP3 server signing off (mailbox empty)\n\r\n");
                            write(*client, print ,strlen(print)); 
                        
                        }
                        else{
                            char print[256];
                            bzero(print,256);
                            strcat(print,"+OK ");
                            strcat(print, username);
                            strcat(print, " POP3 server signing off (");
                            char tmp[1000];
                            bzero(tmp,1000);
                            itoa(left,tmp);
                            strcat(print,tmp);
                            strcat(print, " messages left)\n\r\n");
                            //printf("%s\r\n",print);
                            write(*client, print ,strlen(print)); 
                        }
                    }
                }
                else if (userIn){
                    char print[256];
                    bzero(print,256);
                    strcat(print,"+OK ");
                    strcat(print, username);
                    strcat(print, " POP3 server signing off \r\n");
                    write(*client, print ,strlen(print));
                }
                nextLoop = false;
            }
            else if ( code == 7) {
                int i;
            
                for ( i = 0 ; i< many ; i++){
                emails[i] = true;
                }
                activeStats(username,emails,&plithos,&megethos);
                char stat[256];
                bzero(stat,256);
                char lol[1000];
                bzero(lol,1000);
                strcat(stat,"+OK maildrop has ");
                itoa(plithos,lol);
                strcat(stat,lol);
                strcat(stat," messages (");
                char lol2[1000];
                bzero(lol2,1000);
                itoa(megethos,lol2);
                strcat(stat,lol2);
                strcat(stat," octets)\r\n");
                write(*client,stat,strlen(stat));  
            }
            else if ( code == 8) {
                write(*client, "+OK\r\n" ,strlen("+OK\r\n")); 
            }
            else if ( code == 9) {
                int size = strlen(input) - 4;
                char param1[size];
                int position = 5;
                int c = 0;
                int length = size;
                while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r' && input[position]!=' '){
                    param1[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                
                position++;
                param1[c] = '\0';
                c = 0;
                char param2[size];
                    while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r' && input[position]!=' '){
                    param2[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                param2[c] = '\0';   
                if(c==0){
                write(*client,"-ERR enter a password\r\n" ,strlen("-ERR enter a password\\r\n" ));   
                }
                else if(!userExists(param1)){ 
                mkdir(param1,0777);
                    writePass(param1,param2);
                        char stat[256];
                    bzero(stat,256);
                    strcat(stat,"+OK Welcome ");
                    strcat(stat,param1);
                    strcat(stat,"!\r\n");
                    write(*client,stat,strlen(stat)); 
                }
                else{
                write(*client,"-ERR this user already exist\r\n" ,strlen("-ERR this user already exist\r\n" ));                             
                }
            }
            else if ( code == 10) {
                int size = strlen(input) - 4;
                char param1[size];
                int position = 5;
                int c = 0;
                int length = size;
                while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r' && input[position]!=' '){
                    param1[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                
                position++;
                param1[c] = '\0';
                c = 0;
                char param2[size];
                while ( c < length) {
                    if(input[position] != '\n' && input[position] != '\r' && input[position]!=' '){
                    param2[c] = input[position];
                    c++;
                    position++;
                    }
                    else {
                    break;}
                }
                param2[c] = '\0';  
                if(c==0){
                    write(*client,"-ERR enter a receiver\r\n" ,strlen("-ERR enter a receiver\r\n" ));    
                }
                else if(!userExists(param2) ){
                    write(*client,"-ERR This user does not exist\r\n",strlen("-ERR This user does not exist\r\n"));
                }
                else{
                if(sendEmailTo(username,param1,emails,names,many,param2,*client))
                        write(*client,"+OK email send\r\n",strlen("+OK email send\r\n"));
                } 
            }
               else if(code == 11) {
                         
                         char *help = "THE OPTIONS THAT YOU HAVE ARE: \n 1.USER \n 2.PASS (argument your password)\n 3.STAT \n 4.LIST (maybe with argument)\n 5.RETR (argument)\n 6.DELE (argument) \n 7.QUIT \n 8.NOOP\n 9.RSET\n 10.MAKE (argument user , argument password)\n 11.SEND (argument num of email, user to send the msg) \n 12.HELP\n.\r\n";
                          write(*client,help,strlen(help));
                            
                         }
            else {
                write(*client,"-ERR\r\n",6);
            }
            bzero(input, sizeof input);
            }
        } while (nextLoop); /* Finish on "end" */
        
        free(emails);
        free(names);
        close(*client); /* Close socket */
        printf("Connection from %s is closed\n", rem -> h_name);
}

void sig_handler(int signum,int sock){
  printf("Server Closing\r\n");
  close(sock);
  exit(1);
}

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
      int sp = -1;
    for ( i = 0 ; i < strlen(input); i++){
       if(input[i]==' ') {
         sp = i; 
         break; }
     }
     
     if(sp == -1){
       if(input[4]!='\r'){
         return function;
         }
     }
       else if ( sp !=4 ) {
       return function;
       }
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
    int port,newsock, serverlen;//, clientlen;
    socklen_t clientlen;
    struct sockaddr_in server, client;
    struct sockaddr *serverptr, *clientptr;

    alive_threads = 0;  //Still no active threads

    readConfig(&thread_number,&port);

    /* Create thread pool */
    tpool = (pthread_t*) malloc(thread_number * sizeof(pthread_t));
    for(int i=0 ; i<thread_number ; i++) {
        pthread_create(&tpool[i], NULL, manage_thread, NULL);
    }

    /* The AF_INET address family is the address family for IPv4, AF_INET6 for IPv6, AF_BLUETOOTH for Bluetooth */
    /* IPPROTO_TCP is chosen for the TCP protocol if the type was set to SOCK_STREAM */
    /* IPPROTO_UDP is chosen for the UDP protocol if the type was set to SOCK_DGRAM */
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    { /* Create socket */
        perror("socket");
        exit(1);
    }

    //port = atoi(argv[1]); /* Convert port number to integer */
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
    signal(SIGINT,sig_handler); // Register signal handler
    printf("Listening for connections to port %d\n", port);
    do{ ///sad
        clientptr = (struct sockaddr *) &client;
        clientlen = sizeof client;
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
        {
            perror("accept");
            exit(1);
        } /* Accept connection */

        // if (alive_threads == thread_number-1) {
        //     //close socket
        // }

        //Copy socket
        int *new_req_socket = malloc(sizeof(int));
        *new_req_socket = newsock;

        //Lock threads to push request to queue and increament number of active threads
        pthread_mutex_lock(&mutex);
        enqueue(new_req_socket, &client);
        alive_threads++;
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);   
            
            //exit(0);
    }while(true);
    close(sock);
    return 0;   
}
