#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h> 

#define MAX_PATH 100

bool userExists(const char *username);
bool passwordCheck(char *username,char *passtry);
bool activeStats(const char *username,bool *emails,int *count,int *size );
bool actuallyDeleteFile(const char *username,char *filename);
bool update(const char *username,bool *emails,int *names,int howMany);
bool list(const char *username,int mail, bool *emails,int *names,int howMany);
bool listEmpty(const char *username,bool *emails,int *names,int howMany);
bool fillNames(const char *username,int *names);
bool retrieveMail(int name,char *username);
int fileSize(const char *username, const char *filename);
int howMany(const char *username);
int findMail(int mail,int *names,int howMany);
void deleteMail(int mail,bool *emails,int howMany,int *names);
char* itoa(int i, char b[]);