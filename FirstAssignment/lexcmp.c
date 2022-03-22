#include <string.h>
#include <stdio.h>

int mylencmp(const char *str1, const char *str2)
{
	int val;
	val = strcmp(str1, str2);
	if (val < 0)
		return 1;
	if (val > 0)
		return 2;
	return 0;
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
//test