#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CMD_STR_SIZE 20


void parse(char * str, char ***mds)
{
    int len = strlen(str); //original string length
    int cmdnum = 0;   //matrix index
    int cmdstr_i = 0; //position in word parse
    int newword = 0;  //new word indicator

    char *cmdstr = (char*) malloc(CMD_STR_SIZE * sizeof(char));

    //printf("original string %s", str);

    //mds[cmdnum][cmdstr] = str[i];
    for(int i = 0; i < len; i++)
    {
        if(str[i] == ';')
        {
            break;
        }
        //meets the end of a word 
        else if(str[i] == ' ' || str[i] == '\n')
        {
            //printf("%c ", str[i]);
            //is it really a word?
            if(newword)
            {
                cmdstr_i++;
                cmdstr[cmdstr_i] = '\0';

                (*mds)[cmdnum] = strdup(cmdstr);
                //printf("%s\n", cmdstr);

                free(cmdstr);

                newword = 0;
                cmdstr_i = 0;

                cmdnum++;
            }

            if(str[i] == '\n') break;
            //if not ignore
        }
        //collecting characters
        else
        {
            if(!newword) //start of a new word?
            {
                cmdstr = (char*) malloc(CMD_STR_SIZE * sizeof(char));
                newword = 1;   //new word
            }

            cmdstr[cmdstr_i] = str[i];
            //printf("%c ", cmdstr[cmdstr_i]);
            cmdstr_i++;
        }
    }

    //the array must be null terminated
    (*mds)[cmdnum+1] = NULL;
}

void print_matrix(char **mstr, int len)
{
    for(int i = 0; i < len; i++)
    {
        printf("%s\n", mstr[i]);
    }
}

int loop()
{
    size_t len;
    pid_t child_pid; 

    while(1)
    {
        printf("prompt> ");

        char path_name[100] = "/bin/";
        char **args = (char **) malloc(sizeof(char*) *30);
        char *str = (char*) malloc(512 * sizeof(char));

        getline(&str, &len, stdin);
        printf("%s\n", str);

        if(!strcmp(str, "exit\n")){
            return 0;
        }

        parse(str, &args);
        strcat(path_name, args[0]);
        //printf("all clear\n");
        child_pid = fork();

        if(child_pid == 0){
            //printf("I am a child\n");
            execv(path_name, args);
            exit(0);
        }

        else {
            wait(NULL);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    
    //char **args = (char **) malloc(sizeof(char*) *30);
    //parse("ls -a -s -dsas\n", &args);
    loop();
    return 0;
}
