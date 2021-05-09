#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CMD_STR_SIZE 20

struct StringList_ 
{
    char **data;
    int ocupado;
};
typedef struct StringList_ Stringlist;

Stringlist create_str_list(int size)
{
    Stringlist strlist;

    strlist.data = (char **) malloc(sizeof(char*) * size);
    strlist.ocupado = 0;

    return strlist;
}

void parse(char * str, Stringlist *strlist, char delim)
{
    int len = strlen(str); //original string length
    int list_i = 0;   //matrix index
    int str_i = 0;    //position in string parse
    int new_elem = 0;  //new word indicator

    char *varstr = (char*) malloc(CMD_STR_SIZE * sizeof(char));

    //printf("original string %s", str);

    for(int i = 0; i < len; i++)
    {
        //meets the end of a word 
        if(str[i] == delim || str[i] == '\n')
        {
            //printf("%c ", str[i]);
            //is it really a word?
            if(new_elem)
            {
                str_i++;
                varstr[str_i] = '\0';

                (*strlist).data[list_i] = strdup(varstr);
                //printf("%s\n", cmdstr);

                free(varstr);

                new_elem = 0;
                str_i = 0;

                list_i++;
            }

            if(str[i] == '\n') break;
            //if not ignore
        }
        //collecting characters
        else
        {
            if(!new_elem) //start of a new word?
            {
                varstr = (char*) malloc(CMD_STR_SIZE * sizeof(char));
                new_elem = 1;   //new word
            }

            varstr[str_i] = str[i];
            //printf("%c ", cmdstr[cmdstr_i]);
            str_i++;
        }
    }

    //the array must be null terminated
    (*strlist).ocupado = list_i+1;
    (*strlist).data[list_i+1] = NULL;
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

        Stringlist strlist = create_str_list(30);

        char path_name[100] = "/bin/";
        char *str = (char*) malloc(512 * sizeof(char));

        getline(&str, &len, stdin);

        printf("%s\n", str);
        if(!strcmp(str, "exit\n")){
            return 0;
        }

        parse(str, &strlist, ';');

        pid_t parent_pid = getpid();
        int cmd_num = 0;
        for(int i = 0; i < strlist.ocupado; i++)
        {
            fork();

            if(getpid() != parent_pid)
            {
                cmd_num = i;
                break; 
            }
        }
        printf("i num %d\n", cmd_num);
        if(getpid() != parent_pid)
        {
            Stringlist cmdlist = create_str_list(30);
            parse(strlist.data[cmd_num], &cmdlist, ' ');

            //printf("all clear\n");

            strcat(path_name, cmdlist.data[0]);
            execv(path_name, cmdlist.data);
        }

        else {
            wait(NULL);
        }

        printf("finished\n");
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
