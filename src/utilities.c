/** @file utilities.c
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
/**
 * @brief This function checks if a user exists in our server
 *
 * We check if a directory exists(each user has a directory with its name)
 *
 * EXAMPLE: username: mbofos01 -> directory: mbofos01/
 *
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
 * @param int main ID of a mail (eg 1-> mail number one)
 * @param int *names they array containing the mail IDs
 * @param int howMay total number of emails
 * @return int localized ID (pointer in arrays "names" and "emails")
 * */
int findMail(int mail,int *names,int howMany){
    int i = 0;
    for(i=0;i<howMany;i++){
        if(names[i] == mail)
            return i;
        }
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
 * @param int mail name of the email
 * @param bool *emails dirty bit arrray
 * @param int howMany totalnumber of meails
 * @param int *names the names of the emails
 * @return void
 * */
bool deleteMail(int mail,bool *emails,int howMany,int *names){
    mail = findMail(mail,names,howMany);
    if(mail < howMany && mail >=0)
        emails[mail] = false;
        else
        return false;

    return true;
}
/**
 * @brief This function writes the password of a new user when the make command is called
 *
 * We open a new file (fd), then we add the given password.
 *
 * @param username the name of the new user
 * @param password the password of the new user
 * @return void
 * */
void writePass(char *username,char *password){
    char s1[MAX_PATH];
    strcpy(s1,username);
    strcat(s1,"/PASSWORD");

    int fd = open(s1, O_WRONLY | O_CREAT , 0644);


    if (fd ==-1)
    {
        // print program detail "Success or failure"
        perror("fd write");
    }

    else {
         write(fd,password,strlen(password));
    }

    close(fd);

}
/**
 * @brief This function actually deletes a file from the directory
 *
 * We remove a file from the users directory
 *
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
        if (!emails[i]){
		bool flag = actuallyDeleteFile(username,str);
		if(!flag)
		  return false;
          }
    }
    return true;
}
/**
 * @brief This function collects the names of the emails
 *
 * We see the user directory and collect the names
 *
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
 * @param const char *username
 * @param int *names the names of the emails
 * @return bool if everything its okay true, otherwise false
 * */
bool listEmpty(const char *username,bool *emails,int *names,int howMany,int newsock){
   // printf("=========== EMPTY LIST ===========\n");
    int plithos,megethos;
    activeStats(username,emails,&plithos,&megethos);
    char print[256];
    bzero(print,256);
    strcat(print,"+OK ");
    char lol[1000];
    bzero(lol,1000);
    itoa(plithos,lol);
    strcat(print,lol);
    strcat(print," (messages) (");
    char lol2[1000];
    bzero(lol2,1000);
    itoa(megethos,lol2);
    strcat(print,lol2);
    strcat(print, " octets) \r\n");
    write(newsock,print,strlen(print));
    int i = 0;
    for(i=0;i<howMany;i++)
    	if(emails[i])
	        list(username,names[i],emails,names,howMany,newsock,false);
    write(newsock,". \r\n",4);
    return true;
}
/**
 * @brief This function emulates the LIST signal without a parameter
 *
 * We print the number of an email and its size
 *
 * @bug POSSIBLECHANGE: we have to change the printf to match the server logic
 *
 * @param const char *username
 * @param int mail the name of the email
 * @param int *names the names of the emails
 * @return bool if everything its okay true, otherwise false
 * */
bool list(const char *username,int mail, bool *emails,int *names,int howMany,int newsock,bool isOkay){
    int place = findMail(mail,names,howMany);
    if(place == -1 || emails[place] == false){
        char print[256];
        bzero(print,256);
        strcat(print,"-ERR no such message, only ");
        char lol[1000];
        bzero(lol,1000);
        itoa(howMany,lol);
        strcat(print,lol);
	strcat(print, " in mailbox\r\n");
    	write(newsock,print,strlen(print));
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
    char print[256];
    bzero(print,256);
    if(isOkay)
      strcat(print,"+OK ");
    char lol[1000];
    bzero(lol,1000);
    itoa(mail,lol);
    strcat(print,lol);
    strcat(print," ");
    char lol2[1000];
    bzero(lol2,1000);
    itoa(size,lol2);
    strcat(print,lol2);
    strcat(print, " \r\n");
    write(newsock,print,strlen(print));
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
 * @param int mail name of the email
 * @param const char *username
 * @return bool if everything its okay true, otherwise false
 * */
bool retrieveMail(int name,char *username, bool *emails,int *names,int howMany,int newsock) {
    /****************************** CHECK EXISTANCE ***********************/
    char filename[10];
    char print[256];
    char lol[1000];
    char toPrint[OUTPUT_MESSAGE_SIZE+3];
    char buf[OUTPUT_MESSAGE_SIZE];
    int place = findMail(name,names,howMany);
    printf("Mail %d to retr: %d\r\n",name,place);
    if( place == -1 || emails[place] == false)
    {
    	write(newsock,"-ERR no such message\r\n",strlen("-ERR no such message\r\n"));
    	return false;
    }
    /****************************** GET STATS ****************************/
    bool temp[howMany];
    int i =0;
    for(i=0;i<howMany;i++)
        temp[i] = false;
    temp[place] = true;
    int message_size,null;
    activeStats(username,temp,&null,&message_size);
    /****************************** PRINT STATS **************************/

    bzero(print,256);
    strcat(print,"+OK ");

    bzero(lol,1000);
    itoa(message_size,lol);
    strcat(print,lol);
    strcat(print, " octets\r\n");
    write(newsock,print,strlen(print));

    /****************************** OPEN FILE ***************************/
    itoa(name,filename);
    int name_size = strlen(username)+strlen(filename)+3;
    char s1[name_size];
    strcpy(s1,username);
    strcat(s1,"/");
    strcat(s1,filename);
    s1[name_size-1]='\0';
    int fd = open(s1, O_RDONLY, 0);
    if(fd < 0)
        return false;
    /****************************** READ FILE ***************************/
    int loop_iterations = message_size / OUTPUT_MESSAGE_SIZE + 1;
    for(i=0;i<loop_iterations;i++){

	    bzero(buf,OUTPUT_MESSAGE_SIZE);
	    read(fd, buf, OUTPUT_MESSAGE_SIZE);
	    bzero(toPrint,OUTPUT_MESSAGE_SIZE+3);
	    if(buf[0] == '.')
	    	write(newsock,".",1);
	    write(newsock,buf,strlen(buf));
	    write(newsock,"\r\n",strlen("\r\n"));
    }
    /****************************** CLOSE PRINTING **********************/
    write(newsock,".\r\n",strlen(".\r\n"));
    close(fd);
    return true;
}
/**
 * @brief This function sends a certain email ( given by its id) from one user to another
 *
 * We open a the file one the email that we want to copy to another's users inbox.
 *
 * @param username the name of the user that is going to send the message
 * @param int mail name of the email
 * @param bool *emails dirty bit arrray
 * @param int howMany totalnumber of meails
 * @param int *names the names of the emails
 * @param *to the user that we want to send the email
 * @param socket the number of the open client's socket
 * @return void
 * */
bool sendEmailTo(char *username,char *mail, bool *emails,int *names,int howMany,char *to,int socket){
  int pointer = atoi(mail);
  int place = findMail(pointer,names,howMany);
  char buf[OUTPUT_MESSAGE_SIZE];
  if( place == -1 || emails[place] == false)
    {
    	write(socket,"-ERR no such message\r\n",strlen("-ERR no such message\r\n"));
    	return false;
    }
    /****************************** OPEN FILE ***************************/

    int name_size = strlen(username)+strlen(mail)+3;
    char s1[name_size];
    strcpy(s1,username);
    strcat(s1,"/");
    strcat(s1,mail);
    s1[name_size-1]='\0';
    char TO[name_size];
    strcpy(TO,to);
    strcat(TO,"/");
    strcat(TO,mail);
    TO[name_size-1]='\0';
    int fd = open(s1, O_RDONLY, 0);
    int new_file = open(TO, O_WRONLY | O_CREAT, 0644);
    /****************************** GET STATS ****************************/
    bool temp[howMany];
    int i =0;
    for(i=0;i<howMany;i++)
        temp[i] = false;
    temp[place] = true;
    int message_size,null;
    activeStats(username,temp,&null,&message_size);
    if(fd < 0 || new_file < 0){
        write(socket,"-ERR something went wrong\r\n",strlen("-ERR something went wrong\r\n"));
        return false;
      }
    /****************************** READ FILE ***************************/
    int loop_iterations = message_size / OUTPUT_MESSAGE_SIZE + 1;
    for(i=0;i<loop_iterations;i++){
	    bzero(buf,OUTPUT_MESSAGE_SIZE);
	    read(fd, buf, OUTPUT_MESSAGE_SIZE);
	    write(new_file,buf,strlen(buf));
    }

    close(fd);
    close(new_file);
    return true;
}
/**
 * @brief This function validates a user
 *
 * Because we store the password to a file
 * we read it and compare it with the password
 * given by the user
 *
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
    if(buf[strlen(buf)-1] == '\n')
    	buf[strlen(buf)-1] = '\0';
    close(fd);
    return strcmp(passtry,buf) == 0;
}
