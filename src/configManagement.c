/** @file configManagement.c
 *  @brief This is the c file that helps in the implementation of Hw_4.
 *         It contains all the function implementations as they are declared at h file library.h .
 *         Overall, this c file contains the function implementations that they are helpful for the server functionalities.
 *
 *  You can see below all the function exmplanations.
 *
 *  @author Elia Nicolaou, Pantelis Mikelli, Michail-Panagiotis Mpofos
 *  @version 1
 *  @bug No know bugs.
 *  @see server.c, library.h
 */
#include "library.h"
#define config_file "config.txt"

/**
 * @brief This function checks whether two strings have the same start characters..
 *
 * @param char *str the first string
 * @param char *start the second string
 * @return bool true if they have the same chars/false if they are not.
 * */
bool startsWith(char *str,char *start){
    int i = 0, length = strlen(start);
    for(i=0;i<length;i++){
        if(str[i] != start[i])
            return false;
    }
    return true;
}

/**
 * @brief This function returns the numerical value of a string
 *
 * @param char *str the first string
 * @param char *start the second string
 * @return int the numerical value
 * */
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

/**
 * @brief This function reads the configuration file for the threads/port.
 *
 * @param int *threads  the number of the threads
 * @param int *port     the number of the oport
 * @return void
 * */
void readConfig(int *threads,int *port){
    FILE *fp = fopen(config_file, "r");
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
