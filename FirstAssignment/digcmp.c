#include <string.h>
#include <stdio.h>

int countDigitsInString(const char *str)
{
    int counter = 0;
    char c = '0';
    for (int i = 0; i <= 9; i++)
    {
        if (strchr(str, c) != NULL)
            counter++;
        c++;
    }
    return counter;
}
int mylencmp(const char *str1, const char *str2)
{
    int digCount1 = countDigitsInString(str1);
    int digCount2 = countDigitsInString(str2);
    if (digCount1 != digCount2)
    {
        if (digCount1 > digCount2)
            return 2;
        else
            return 1;
    }
    else
        return 0;
    return -1;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("need 2 arguments");
        return -1;
    }
    int answer = mylencmp(argv[1], argv[2]);
    return answer;
}