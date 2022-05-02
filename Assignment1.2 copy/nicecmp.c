#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define LINELEN (80)
#define LENCMP 1
#define LEXCMP 0

char *mygets(char *buf, int len);
int mygeti();

int main(int argc, char *argv[])
{
	char *cmpstr[] = {"lexcmp", "lencmp"};
	char str1[LINELEN + 1];
	char str2[LINELEN + 1];
	int index;
	int veclen = sizeof(cmpstr) / sizeof(char *);

	// LEX PIPES CREATE

	int lexPipeReceiveFD[2];
	if (pipe(lexPipeReceiveFD) != 0)
		return -2;

	int lexPipeTransmitFD[2];
	if (pipe(lexPipeTransmitFD) != 0)
		return -2;

	int lexProccesID = fork();

	if (lexProccesID == 0)
	{
		// SET TRANSMIT PIPE - WRITES TO LOOPCMP LEXCMP
		if (dup2(lexPipeTransmitFD[0], STDIN_FILENO) == -1)
			return -2;
		if (close(lexPipeTransmitFD[0]) != 0)
			return -2;
		if (close(lexPipeTransmitFD[1]) != 0)
			return -2;

		//  SET RECIVE PIPE - READS FROM LOOPCMP LEXCMP
		if (dup2(lexPipeReceiveFD[1], STDOUT_FILENO) == -1)
			return -2;
		if (close(lexPipeReceiveFD[0]) != 0)
			return -2;
		if (close(lexPipeReceiveFD[1]) != 0)
			return -2;

		char *myargs[3] = {"./loopcmp", "lexcmp", NULL};
		if (execvp(myargs[0], myargs) == -1)
			return -2;
	}

	if (close(lexPipeTransmitFD[0]) != 0)
		return -2;
	if (close(lexPipeReceiveFD[1]) != 0)
		return -2;

	// LEN PIPES CREATE

	int lenPipeReceiveFD[2];
	if (pipe(lenPipeReceiveFD) != 0)
		return -2;

	int lenPipeTransmitFD[2];
	if (pipe(lenPipeTransmitFD) != 0)
		return -2;

	int lenProccesID = fork();

	if (lenProccesID == 0)
	{
		if (close(lexPipeTransmitFD[1]) != 0)
			return -2;
		if (close(lexPipeReceiveFD[0]) != 0)
			return -2;

		// SET TRANSMIT PIPE - WRITES TO LOOPCMP LENCMP
		if (dup2(lenPipeTransmitFD[0], STDIN_FILENO) == -1)
			return -2;
		if (close(lenPipeTransmitFD[0]) != 0)
			return -2;
		if (close(lenPipeTransmitFD[1]) != 0)
			return -2;

		//  SET RECIVE PIPE - READS FROM LOOPCMP LENCMP
		if (dup2(lenPipeReceiveFD[1], STDOUT_FILENO) == -1)
			return -2;
		if (close(lenPipeReceiveFD[0]) != 0)
			return -2;
		if (close(lenPipeReceiveFD[1]) != 0)
			return -2;

		char *myargs[3] = {"./loopcmp", "lencmp", NULL};

		if (execvp(myargs[0], myargs) == -1)
			return -2;
	}

	if (close(lenPipeTransmitFD[0]) != 0)
		return -2;
	if (close(lenPipeReceiveFD[1]) != 0)
		return -2;
	// START LOOP
	while (1)
	{
		// GET STRINGS
		printf("Please enter first string:\n");
		if (mygets(str1, LINELEN) == NULL)
			break;

		printf("Please enter second string:\n");

		if (mygets(str2, LINELEN) == NULL)
			break;

		// GET COMPARE TYPE
		do
		{
			printf("Please choose:\n");
			for (int i = 0; i < veclen; i++)
				printf("%d - %s\n", i, cmpstr[i]);
			index = mygeti();
		} while ((index < 0) || (index >= veclen));

		char returnFromLoopcmp;
		// CHANNEL THE PIPE TO THE SELECTED COPMARATOR
		switch (index)
		{
		case LEXCMP:
			if (write(lexPipeTransmitFD[1], str1, strlen(str1)) != strlen(str1))
				return -2;
			if (write(lexPipeTransmitFD[1], "\n", 1) != 1)
				return -2;
			if (write(lexPipeTransmitFD[1], str2, strlen(str2)) != strlen(str2))
				return -2;
			if (write(lexPipeTransmitFD[1], "\n", 1) != 1)
				return -2;

			if (read(lexPipeReceiveFD[0], &returnFromLoopcmp, 2) != 2)
				return -2;
			break;

		case LENCMP:
			if (write(lenPipeTransmitFD[1], str1, strlen(str1)) != strlen(str1))
				return -2;
			if (write(lenPipeTransmitFD[1], "\n", 1) != 1)
				return -2;
			if (write(lenPipeTransmitFD[1], str2, strlen(str2)) != strlen(str2))
				return -2;
			if (write(lenPipeTransmitFD[1], "\n", 1) != 1)
				return -2;

			if (read(lenPipeReceiveFD[0], &returnFromLoopcmp, 2) != 2)
				return -2;
			break;
		}

		printf("%s(%s, %s) == %c\n", cmpstr[index], str1, str2, returnFromLoopcmp);
		fflush(stdout);
	}

	// close lexcmp

	if (close(lexPipeTransmitFD[1]) != 0)
		return -2;
	if (close(lexPipeReceiveFD[0]) != 0)
		return -2;

	int childReturns;
	if (waitpid(lexProccesID, &childReturns, 0) == -1)
		return -2;

	if (!WIFEXITED(childReturns))
		return -2;

	int exitStatus = WEXITSTATUS(childReturns);

	// close lencmp

	if (close(lenPipeTransmitFD[1]) != 0)
		return -2;
	if (close(lenPipeReceiveFD[0]) != 0)
		return -2;

	if (waitpid(lenProccesID, &childReturns, 0) == -1)
		return -2;

	if (!WIFEXITED(childReturns))
		return -2;

	exitStatus = WEXITSTATUS(childReturns);

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

int mygeti()
{
	int ch;
	int retval = 0;

	while (isspace(ch = getchar()))
		;
	while (isdigit(ch))
	{
		retval = retval * 10 + ch - '0';
		ch = getchar();
	}
	while (ch != '\n')
		ch = getchar();
	return retval;
}