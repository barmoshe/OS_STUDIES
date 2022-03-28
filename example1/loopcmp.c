#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINELEN (80)

int lencmp(const char *str1, const char *str2);
int lexcmp(const char *str1, const char *str2);
int digcmp(const char *str1, const char *str2);
char *mygets(char *buf, int len);

int main(int argc, char *argv[])
{
	int (*cmpfunc)(const char *, const char *) = NULL;
	char str1[LINELEN + 1];
	char str2[LINELEN + 1];

	if (argc != 2)
		return -1;

	if (!strcmp(argv[1], "lexcmp"))
		cmpfunc = lexcmp;
	else if (!strcmp(argv[1], "lencmp"))
		cmpfunc = lencmp;
	else if (!strcmp(argv[1], "digcmp"))
		cmpfunc = digcmp;
	else
		return -1;

	while (1)
	{
		if (mygets(str1, LINELEN) == NULL)
			break;
		if (mygets(str2, LINELEN) == NULL)
			break;
		int answer = cmpfunc(str1, str2);
		printf("Answer:\t%d\n", answer);
		fflush(stdout);
	}
	return 0;
}

int lencmp(const char *str1, const char *str2)
{
	int val;
	val = strlen(str1) - strlen(str2);
	if (val < 0)
		return 1;
	if (val > 0)
		return 2;
	return 0;
}

int lexcmp(const char *str1, const char *str2)
{
	int val;

	val = strcmp(str1, str2);
	if (val < 0)
		return 1;
	if (val > 0)
		return 2;
	return 0;
}
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
}
char *mygets(char *buf, int len)
{
	char *retval;
	printf("enter string : \n");
	retval = fgets(buf, len, stdin);
	buf[len] = '\0';
	if (buf[strlen(buf) - 1] == 10) /* trim \r */
		buf[strlen(buf) - 1] = '\0';
	else if (retval)
		while (getchar() != '\n')
			; /* get to eol */

	return retval;
}