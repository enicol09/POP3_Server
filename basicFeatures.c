#include "library.h"
/**
 * Driver class to see the use of the mail utilities
 * */
int main(int argc, char* argv[])
{
    bool transaction = false;
    bool userIn = false;
    bool nextLoop = true;
    char *inputs[] = {"1","mbofos01","9","1111","3","1","7"};
    char *folder= NULL;
    bool *emails = NULL;
    int *names = NULL;
    int plithos,megethos,many = -1;
    int counter = 0;

    do{
        int code = atoi(inputs[counter]);
        if(code == 1){
            printf("----enter user\n");
            userIn = true;
            folder = inputs[counter+1];
            many = howMany(inputs[code]);
            emails = malloc(sizeof(bool)*many);
            int i =0;
            for(i=0;i<many;i++)
                emails[i] = true;
            names = malloc(sizeof(int)*many);
            fillNames(folder,names);
        }
        else if(userIn && !transaction && code == 9){
            if(passwordCheck(folder,inputs[counter+1])){
                transaction = true;
            }
            else{
                nextLoop = false;
            }
            counter++;
        }
        else if(transaction && code == 2){
            printf("----get stat\n");
            activeStats(folder,emails,&plithos,&megethos);
            printf("How many: %d and size %d\n",plithos,megethos);
        }
        else if(transaction && code == 3){
            printf("----delete: %d\n",atoi(inputs[counter+1]));
            deleteMail(atoi(inputs[counter+1]),emails,many,names);
            counter++;
        }
        else if(transaction && code == 4){
            printf("----see empty list\n");
            listEmpty(folder,emails,names,many);
        }
        else if(transaction && code == 5){
            printf("----updating\n");
            update(folder,emails,names,many);
        }
        else if(transaction && code == 6){
            printf("----list of %d\n",atoi(inputs[counter+1]));
            counter++;
            list(folder,atoi(inputs[counter]),emails,names,many);
        }
        else if(transaction && code == 7){
            printf("----quit\n");
            update(folder,emails,names,many);
            nextLoop = false;
        }
        else if(transaction && code == 8){
            counter++;
            printf("----print mail number %d\n",atoi(inputs[counter]));
            retrieveMail(atoi(inputs[counter]),folder);
        }
       
        counter++;

    }while(nextLoop);
        free(emails);
        free(names);
}
