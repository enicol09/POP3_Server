#include "library.h"
/**
 * @brief This function checks if a user exists in our server
 * 
 * We check if a directory exists(each user has a directory with its name)
 * 
 * EXAMPLE: username: mbofos01 -> directory: mbofos01/
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username
 * @return bool true if valid user, otherwise false
 * 
 * */
bool userExists(const char *username){
    struct stat sb;
    if (stat(username, &sb) == 0 && S_ISDIR(sb.st_mode)) {
       return true;
    }
    return false;
}
/**
 * @brief This function gets the size of an email
 * 
 * We need the username so we can enter the directory 
 * and then inspect the file
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username
 * @param const char *filename
 * @return int size in bytes
 * */
int fileSize(const char *username, const char *filename){
    struct stat st;
    char s1[MAX_PATH];
    strcpy(s1,username);
    strcat(s1,"/");
    strcat(s1,filename);
    stat(s1, &st);
    return st.st_size;
}
/**
 * @brief This function counts the emails in a directory
 * 
 * Warning each user directory must have a PASSWORD file
 * which contains the password of the user
 * 
 * @bug security breach
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username
 * @return int number of emails
 * */
int howMany(const char *username){
   size_t count = 0;
    struct dirent *res;
    struct stat sb;

    if (stat(username, &sb) == 0 && S_ISDIR(sb.st_mode)){
        DIR *folder = opendir ( username );
        if (access ( username, F_OK ) != -1 ){
            if ( folder ){
                while ( ( res = readdir ( folder ) ) ){
                    if ( strcmp( res->d_name, "." ) && strcmp( res->d_name, ".." ) && strcmp( res->d_name, "PASSWORD" )){
                        count++;
                    }
                }
                closedir (folder);
            }else{
                perror ( "Could not open the directory" );
                return -1;
            }
        }
    }else{
        //printf("The %s it cannot be opened or is not a directory\n", username);
         return -1;
    }

    return count;
}
/**
 * @brief This function calculates the number of active emails and their size
 * 
 * We check if a mail is active and increase a counter and add its size
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username
 * @param bool *emails active/deleted emails
 * @param int *count int pointer for the number of emails
 * @param int *size int pointer for the size of emails
 * @return bool if everything its okay true, otherwise false
 * */
bool activeStats(const char *username,bool *emails,int *count,int *size ){
    *count = 0;
    *size = 0;
    struct dirent *res;
    struct stat sb;

    if (stat(username, &sb) == 0 && S_ISDIR(sb.st_mode)){
        DIR *folder = opendir ( username );
        if (access ( username, F_OK ) != -1 ){
            if ( folder ){
                int pointer = 0;
                while ( ( res = readdir ( folder ) ) ){
                    if ( strcmp( res->d_name, "." ) && strcmp( res->d_name, ".." )&& strcmp( res->d_name, "PASSWORD" ) && emails[pointer++]){
                        (*count)++;
                        (*size) +=fileSize(username,res->d_name);
                    }
                }
                closedir (folder);
            }else{
                perror ( "Could not open the directory" );
                return false;
            }
        }
    }else{
        //printf("The %s it cannot be opened or is not a directory\n", username);
         return false;
    }

    return true;
}
/**
 * @brief This function finds the pointer of an email
 * 
 * Because every email names is a number we have to find its
 * place in the active/deleted bool array
 * 
 * @author Michail-Panagiotis Bofos
 * @param int main ID of a mail (eg 1-> mail number one)
 * @param int *names they array containing the mail IDs
 * @param int howMay total number of emails
 * @return int localized ID (pointer in arrays "names" and "emails")
 * */
int findMail(int mail,int *names,int howMany){
    int i = 0;
    for(i=0;i<howMany;i++)
        if(names[i] == mail)
            return i;
    return -1;
}
/**
 * @brief This function marks an email as deleted
 * 
 * We operate as a CPU, we use a dirty bit, using the 
 * array emails,if a value is false this email is not active
 * and if we end up updating our state it will be actually deleted
 * from the users directory
 * 
 * @author Michail-Panagiotis Bofos
 * @param int mail name of the email
 * @param bool *emails dirty bit arrray
 * @param int howMany totalnumber of meails
 * @param int *names the names of the emails
 * @return void
 * */
void deleteMail(int mail,bool *emails,int howMany,int *names){
    mail = findMail(mail,names,howMany);
    if(mail < howMany && mail >=0)
        emails[mail] = false;
}
/**
 * @brief This function actually deletes a file from the directory
 * 
 * We remove a file from the users directory
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username 
 * @param char *filename 
 * @return true if everything went okay, otherwise false
 * */
bool actuallyDeleteFile(const char *username,char *filename){
    char s1[MAX_PATH];
    strcpy(s1,username);
    strcat(s1,"/");
    strcat(s1,filename);
    int del = remove(s1);
    if (!del)
      return true;
    else
      return false;
}
/**
 * @brief Reverse of atoi
 * 
 * This function turns an integer to string
 * 
 * @author Michail-Panagiotis Bofos
 * @param int i number we want to change
 * @param char b[] the string we create
 * @return char *
 * */
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
/**
 * @brief UPDATE function of the server
 * 
 * Actually we just delete the marked emails
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username 
 * @param bool *emails dirty bit arrray
 * @param int *names the names of the emails
 * @param int howMany totalnumber of meails
 * @return bool if everything its okay true, otherwise false
 * */
bool update(const char *username,bool *emails,int *names,int howMany){
    int i =0;
    for(i=0;i<howMany;i++){
        char str[1000];
        itoa(names[i],str);
        if (!emails[i])
            actuallyDeleteFile(username,str);
    }
    return true;
}
/**
 * @brief This function collects the names of the emails
 * 
 * We see the user directory and collect the names
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username 
 * @param int *names the names of the emails
 * @return bool if everything its okay true, otherwise false
 * */
bool fillNames(const char *username,int *names){
    struct dirent *res;
    struct stat sb;

    if (stat(username, &sb) == 0 && S_ISDIR(sb.st_mode)){
        DIR *folder = opendir ( username );
        if (access ( username, F_OK ) != -1 ){
            if ( folder ){
                int pointer = 0;
                while ( ( res = readdir ( folder ) ) ){
                    if ( strcmp( res->d_name, "." ) && strcmp( res->d_name, ".." )&& strcmp( res->d_name, "PASSWORD" )){
                        names[pointer++] = atoi(res->d_name);
                        //printf("File: %d\n",names[pointer-1]);
                    }
                }
                closedir (folder);
            }else{
                perror ( "Could not open the directory" );
                return false;
            }
        }
    }else{
        //printf("The %s it cannot be opened or is not a directory\n", username);
         return false;
    }

    return true;
}
/**
 * @brief This function emulates the LIST signal without any parameter
 * 
 * We print the number of each email and its size
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username 
 * @param int *names the names of the emails
 * @return bool if everything its okay true, otherwise false
 * */
bool listEmpty(const char *username,bool *emails,int *names,int howMany){
   // printf("=========== EMPTY LIST ===========\n");
    int i = 0;
    for(i=0;i<howMany;i++)
        list(username,names[i],emails,names,howMany);
    return true;
}
/**
 * @brief This function emulates the LIST signal without a parameter
 * 
 * We print the number of an email and its size
 * 
 * @bug POSSIBLECHANGE: we have to change the printf to match the server logic
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username 
 * @param int mail the name of the email
 * @param int *names the names of the emails
 * @return bool if everything its okay true, otherwise false
 * */
bool list(const char *username,int mail, bool *emails,int *names,int howMany){
    int place = findMail(mail,names,howMany);
    if(!(place < howMany && place >=0 && emails[place])){
        return false;
    }
    bool temp[howMany];
    int i =0;
    for(i=0;i<howMany;i++)
        temp[i] = false;
    temp[place] = true;
    int size,null;
    activeStats(username,temp,&null,&size);
    //printf("%d %d\n",names[place],size);
    return true;
}
/**
 * @brief This function retrieves an email
 * 
 * We open the email file and print it
 * 
 * @bug POSSIBLECHANGE: We will need to break the email to chuncks 
 * and again replace the printf to server logic
 * 
 * @author Michail-Panagiotis Bofos
 * @param int mail name of the email
 * @param const char *username 
 * @return bool if everything its okay true, otherwise false
 * */
bool retrieveMail(int name,char *username) {
    char filename[10];

    itoa(name,filename);
    //printf("%s\n", filename);
    int size = strlen(username)+strlen(filename)+3;
    char s1[size];
    strcpy(s1,username);
    strcat(s1,"/");
    strcat(s1,filename);
    s1[size-1]='\0';
    //printf("%s\n", s1);
    int fd = open(s1, O_RDONLY, 0);
    if(fd < 0)
        return false;
    //printf("%d\n", fd);
    char buf[50000];
    read(fd, buf, 50000-1);
    //buf[50000-1] = '\0';
    //printf("%s\n", buf);
    return true;
}
/**
 * @brief This function validates a user
 * 
 * Because we store the password to a file
 * we read it and compare it with the password
 * given by the user
 * 
 * @author Michail-Panagiotis Bofos
 * @param const char *username
 * @param char *passtry possible password
 * @return bool if everything its okay true, otherwise false 
 * */
bool passwordCheck(char *username,char *passtry){
    int size = strlen(username)+strlen("PASSWORD")+3;
    char s1[size];
    strcpy(s1,username);
    strcat(s1,"/");
    strcat(s1,"PASSWORD");
    s1[size-1]='\0';
    int fd = open(s1, O_RDONLY, 0);
    if(fd < 0)
        return false;
    //printf("%d\n", fd);
    char buf[100];
    read(fd, buf, 100-1);
    return strcmp(passtry,buf) == 0;
}