#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

void parseargs(char *argv[], int argc, int *lval, int *uval, int *nval, int *tval);
int isprime(int n);
char *flagarr = NULL;
int lval = 1;
int uval = 100;
int num;

int main(int argc, char **argv)
{
    int nval = 10;
    int tval = 4;
    int countPrime = 0;

    // Parse arguments
    parseargs(argv, argc, &lval, &uval, &nval, &tval);
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
            countPrime++;
        }
        else
        {
            flagarr[num - lval] = 0;
        }
    }

    // Print results
    printf("Found %d primes%c\n", countPrime, countPrime ? ':' : '.');
    printf("printing %d : \n", nval);
    int temp = nval;
    int index = 1;
    for (int i = lval; i <= uval; i++)
        if (flagarr[i - lval])
        {
            temp--;
            if (temp >= 0)
            {
                printf("%d%c) %d\n", index, index >= 10 ? '\0' : ' ', i);
                index++;
            }
        }

    return 0;
}

// NOTE : use 'man 3 getopt' to learn about getopt(), opterr, optarg and optopt
void parseargs(char *argv[], int argc, int *lval, int *uval, int *nval, int *tval)
{
    int ch;

    opterr = 0;
    while ((ch = getopt(argc, argv, "l:u:n:t:")) != -1)
        switch (ch)
        {
        case 'l': // Lower bound flag
            *lval = atoi(optarg);
            break;
        case 'u': // Upper bound flag
            *uval = atoi(optarg);
            break;
        case 'n': // Upper bound flag
            *nval = atoi(optarg);
            break;
        case 't':
            *tval = atoi(optarg);
        case '?':
            if (((optopt == 'l') || (optopt == 'u')) || (optopt == 'n'))
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