#include <string.h>
#include <stdio.h>
#include <ctype.h>
int countDigitsInString(const char *s)
{
    int length = strlen(s);
    int count = 0;
    for (int i = 0; i < length; i++)
        if (s[i] >= '0' && s[i] <= '9')
            count++;

    return count;
}

int digcmp(const char *str1, const char *str2)
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
    int answer = digcmp(argv[1], argv[2]);
    printf("Answer:\t%d\n", answer);
    return answer;
}