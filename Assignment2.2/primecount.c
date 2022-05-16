#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

void parseargs(char *argv[], int argc, int *lval, int *uval);
int isprime(int n);
char *flagarr = NULL;
int lval = 1;
int uval = 100;
int num;

int main(int argc, char **argv)
{
    int lval = 1;
    int uval = 100;
    int count = 0;

    // Parse arguments
    parseargs(argv, argc, &lval, &uval);
    if (uval < lval)
    {
        fprintf(stderr, "Upper bound should not be smaller then lower bound\n");
        exit(1);
    }
    if (lval < 2)
    {
        lval = 2;
        uval = (uval > 1) ? uval : 1;
    }

    // Allocate flags
    flagarr = (char *)malloc(sizeof(char) * (uval - lval + 1));
    if (flagarr == NULL)
        exit(1);

    // Set flagarr
    for (num = lval; num <= uval; num++)
    {
        if (isprime(num))
        {
            flagarr[num - lval] = 1;
            count++;
        }
        else
        {
            flagarr[num - lval] = 0;
        }
    }

    // Print results
    printf("Found %d primes%c\n", count, count ? ':' : '.');
    for (num = lval; num <= uval; num++)
        if (flagarr[num - lval])
        {
            count--;
            printf("%d%c", num, count ? ',' : '\n');
        }
    return 0;
}

// NOTE : use 'man 3 getopt' to learn about getopt(), opterr, optarg and optopt
void parseargs(char *argv[], int argc, int *lval, int *uval)
{
    int ch;

    opterr = 0;
    while ((ch = getopt(argc, argv, "l:u:")) != -1)
        switch (ch)
        {
        case 'l': // Lower bound flag
            *lval = atoi(optarg);
            break;
        case 'u': // Upper bound flag
            *uval = atoi(optarg);
            break;
        case '?':
            if ((optopt == 'l') || (optopt == 'u'))
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            exit(1);
        default:
            exit(1);
        }
}

int isprime(int n)
{
    int i, root;

    if ((n != 2) && (n % 2 == 0))
        return 0;

    root = (int)(sqrt(n));

    for (i = 3; i <= root; i += 2)
        if (n % i == 0)
            return 0;

    return 1;
}