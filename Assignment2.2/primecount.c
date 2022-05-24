#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

void parseargs(char *argv[], int argc, int *lval, int *uval, int *nval, int *pval, int *tval);
int isprime(int n);

int globalNum;

struct argument
{
    char *flagarr;
    int lval;
    int uval;
    pthread_mutex_t mutex;
    int index;
};

void *func(void *arguments)
{
    struct argument *args = (struct argument *)arguments;
    while (globalNum <= args->uval)
    {
        pthread_mutex_lock(&args->mutex);
        int mynum = globalNum;
        globalNum++;
        pthread_mutex_unlock(&args->mutex);
        if (isprime(mynum))
        {
            args->flagarr[mynum - args->lval] = 1;
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    int lval = 1;
    int uval = 1000;
    int nval = 10;
    int pval = 4;
    int tval = 4;
    char *flagarr = NULL;
    pthread_t *threads = NULL;
    int count = 0;

    parseargs(argv, argc, &lval, &uval, &nval, &pval, &tval);
    if (uval < lval)
    {
        fprintf(stderr, "Upper Erorr\n");
        exit(1);
    }
    if (lval < 2)
    {
        lval = 2;
        uval = (uval > 1) ? uval : 1;
    }
    if (pval <= 0)
    {
        fprintf(stderr, "You need to use at least one processor\n");
        exit(1);
    }
    if (nval <= 0)
    {
        fprintf(stderr, "You need to use at least one print\n");
        exit(1);
    }
    if (tval <= 0)
    {
        fprintf(stderr, "You need to use at least one thread\n");
        exit(1);
    }

    flagarr = (char *)calloc((uval - lval + 1), sizeof(char));
    if (flagarr == NULL)
        exit(1);

    threads = (pthread_t *)malloc(sizeof(pthread_t) * tval);
    if (threads == NULL)
    {
        fprintf(stderr, "pthread unsuccess\n");
        exit(1);
    }

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    struct argument args;
    args.flagarr = flagarr;
    args.lval = lval;
    args.uval = uval;
    args.mutex = mutex;

    for (int i = 0; i < tval; i++)
    {
        args.index = i;
        if (pthread_create(threads + i, NULL, &func, (void *)&args) != 0)
            exit(1);
    }

    for (int i = 0; i < tval; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
            perror("Thread error\n");
    }

    for (int i = 0; i < uval; i++)
    {
        if (flagarr[i] == 1)
            count++;
    }

    printf("Found %d primes%c\n", count, count ? ':' : '.');
    for (globalNum = lval; (globalNum <= uval) && (nval != 0); globalNum++)
        if (flagarr[globalNum - lval])
        {
            count--;
            nval--;
            printf("%d%c", globalNum, nval ? ',' : '\n');
        }

    pthread_mutex_destroy(&mutex);
    return 0;
}

void parseargs(char *argv[], int argc, int *lval, int *uval, int *nval, int *pval, int *tval)
{
    int ch;

    opterr = 0;
    while ((ch = getopt(argc, argv, "l:u:n:p:t:")) != -1)
        switch (ch)
        {
        case 'l':
            *lval = atoi(optarg);
            break;
        case 'u':
            *uval = atoi(optarg);
            break;
        case 'n':
            *nval = atoi(optarg);
            break;
        case 'p':
            *pval = atoi(optarg);
            break;
        case 't':
            *tval = atoi(optarg);
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
