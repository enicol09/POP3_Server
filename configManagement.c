#include "library.h"

bool startsWith(char *str,char *start){
    int i = 0, length = strlen(start);
    for(i=0;i<length;i++){
        if(str[i] != start[i])
            return false;
    }
    return true;
}
int getValue(char *str,char *start){
    int i = 0, length = strlen(str),cnt=0;
    char num[100];
    for(i=strlen(start)+1;i<length;i++){
        if(isdigit(str[i]))
            num[cnt++] = str[i];
    }
    num[cnt] = '\0';
    return atoi(num);
}

void readConfig(int *threads,int *port){
    FILE *fp = fopen("config.txt", "r");
     if(fp == NULL) {
         perror("Unable to open file!");
         exit(1);
     }
 
     char chunk[128];
     while(fgets(chunk, sizeof(chunk), fp) != NULL) {
         //fputs(chunk, stdout);
         if(startsWith(chunk,"THREADS")){
             //printf("%s\n",chunk);
             *threads = getValue(chunk,"THREADS");
             
         }
         else if(startsWith(chunk,"PORT")){
             //printf("%s\n",chunk);
             *port = getValue(chunk,"PORT");
         }
     }
     fclose(fp);
}

/**

void main(){
    int threads,port;
    readConfig(&threads,&port);
    printf("T: %d P: %d\n",threads,port);
}
**/