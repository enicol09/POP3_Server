#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include<signal.h>
#include <fcntl.h> 

#define MAX_PATH 100
#define OUTPUT_MESSAGE_SIZE 512

bool deleteMail(int mail,bool *emails,int howMany,int *names);
bool userExists(const char *username);
bool passwordCheck(char *username,char *passtry);
bool activeStats(const char *username,bool *emails,int *count,int *size );
bool actuallyDeleteFile(const char *username,char *filename);
bool update(const char *username,bool *emails,int *names,int howMany);
bool list(const char *username,int mail, bool *emails,int *names,int howMany,int newsock,bool isOkay);
bool listEmpty(const char *username,bool *emails,int *names,int howMany,int newsock);
bool fillNames(const char *username,int *names);
bool retrieveMail(int name,char *username, bool *emails,int *names,int howMany,int newsock);
bool startsWith(char *str,char *start);
bool sendEmailTo(char *username,char *mail, bool *emails,int *names,int howMany,char *to,int socket);

int fileSize(const char *username, const char *filename);
int howMany(const char *username);
int findMail(int mail,int *names,int howMany);

char* itoa(int i, char b[]);

int getValue(char *str,char *start);

void readConfig(int *threads,int *port);
void writePass(char *username,char *password);
