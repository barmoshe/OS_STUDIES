#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

#define LINELEN (80)

char *mygets(char *buf, int len);
char *progName(char *prog);
int main(int argc, char *argv[])
{
    char str1[LINELEN + 1];
    char str2[LINELEN + 1];

    if (argc != 2)
        return -1;

    while (1)
    {
        if (mygets(str1, LINELEN) == NULL)
            break;

        if (mygets(str2, LINELEN) == NULL)
            break;

        int processId = fork();

        if (processId == 0)
        {
            char *myargs[4];
            char *prog = progName(argv[1]);
            myargs[0] = prog;
            myargs[1] = strdup(str1);
            myargs[2] = strdup(str2);
            myargs[3] = NULL;

            if (execvp(myargs[0], myargs) == -1)
                return -2;

            printf("this shouldn't print out");
        }
        else
        {
            int childReturns;
            if (wait(&childReturns) == -1)
                return -2;

            if (!WIFEXITED(childReturns))
                return -2;

            int ChildExitStatus = WEXITSTATUS(childReturns);

            printf("%d\n", ChildExitStatus);
            fflush(stdout);
        }
    }

    return 0;
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
char *progName(char *prog)
{
    char *filePath = (char *)malloc(strlen(prog) + 3);
    if (!filePath)
        return NULL;
    strcpy(filePath, "./");
    strcat(filePath, prog);
    return filePath;
}
