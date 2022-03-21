#include <string.h>
#include <stdio.h>

//
int mylencmp(const char *str1, const char *str2)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if (len1 != len2)
    {
        if (len1 > len2)
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