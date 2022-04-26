#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

#define LINELEN (80)

char *mygets(char *buf, int len);
char *progName(char *prog);
int main(int argc, char *argv[])
{
    char *args[4];
    args[3] = NULL;
    char str1[LINELEN + 1];
    char str2[LINELEN + 1];
    int childReturns = 1;
    if (argc != 2)
        return -2;

    char *prog = progName(argv[1]);
    if (prog == NULL)
        return -2;
    args[0] = prog;
    while (1)
    {
        printf("Enter string:");
        if (mygets(str1, LINELEN) == NULL)
            break;
        printf("Enter  string:");
        if (mygets(str2, LINELEN) == NULL)
            break;
        args[1] = str1;
        args[2] = str2;
        int processId = fork();
        if (processId == 0)
        {
            if (execvp(prog, args) > 0)
                return -2;
        }
        else if (processId > 0)
        {
            wait(&childReturns); // Wait for the child
            printf("Child  code is %d\n", WEXITSTATUS(childReturns));
        }
        else
            return -2;
    }
    free(prog);
    return 0;
}
char *progName(char *prog)
{
    char *filePath = (char *)malloc(strlen(prog) + 3);
    if (!filePath)
        return NULL;
    strcpy(filePath, "./");
    strcat(filePath, prog);
    return filePath;
}

char *mygets(char *buf, int len)
{
    char *retval;

    retval = fgets(buf, len, stdin);
    buf[len] = '\0';
    if (buf[strlen(buf) - 1] == 10) /* trim \r */
        buf[strlen(buf) - 1] = '\0';
    else if (retval)
        while (getchar() != '\n')
            ; /* get to eol */

    return retval;
}