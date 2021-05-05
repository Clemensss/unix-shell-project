#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int loop()
{
    char *str = (char*) malloc(512 * sizeof(char));
    size_t len;
    pid_t child_pid; 
    while(1)
    {
        printf("WOW>");
        getline(&str, &len, stdin);
        str[strlen(str)-1] = '\0';
        printf("%s\n", str);

        if(!strcmp(str, "exit\n")){
            return 0;
        }

        child_pid = fork();

        if(child_pid == 0){
            printf("I am a child\n");
            execlp(str, str, NULL, NULL);
            exit(0);
        }
        else {
            wait(NULL);
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    loop();
    return 0;
}
