/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the main loop, including dividing input into words.
 *
 * You should not need to modify this file. You may invoke its functions if you like, however.
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"

/* word delimiters */
const char *delimiters = " ?\t\n";


/*
 * Main loop.
 */
int main(int argc, char *argv[]) {

	char input[MAX_INPUT];      /* buffer for holding the user input */
	int inc;                    /* the number of words in the user input */
	char *inv[MAX_INPUT];       /* pointers to the beginning of each word of input */
	char output[MAX_RESPONSE];  /* the chatbot's output */
	int len;                    /* length of a word */
	int done = 0;               /* set to 1 to end the main loop */

	/* initialise the chatbot */
	inv[0] = "reset";
	inv[1] = NULL;
	chatbot_do_reset(1, inv, output, MAX_RESPONSE);

	/* print a welcome message */
	printf("%s: Hello, I'm %s.\n", chatbot_botname(), chatbot_botname());

	/* main command loop */
	do {

		do {
			/* read the line */
			printf("%s: ", chatbot_username());
			fgets(input, MAX_INPUT, stdin);

			/* split it into words */
			inc = 0;
			inv[inc] = strtok(input, delimiters);
			while (inv[inc] != NULL) {

				/* remove trailing punctuation */
				len = strlen(inv[inc]);
				while (len > 0 && ispunct(inv[inc][len - 1])) {
					inv[inc][len - 1] = '\0';
					len--;
				}

				/* go to the next word */
				inc++;
				inv[inc] = strtok(NULL, delimiters);
			}
		} while (inc < 1);

		/* invoke the chatbot */
		done = chatbot_main(inc, inv, output, MAX_RESPONSE);
		printf("%s: %s\n", chatbot_botname(), output);

	} while (!done);

	return 0;
}


/*
 * Utility function for comparing string case-insensitively.
 *
 * Input:
 *   token1 - the first token
 *   token2 - the second token
 *
 * Returns:
 *   as strcmp()
 */
int compare_token(const char *token1, const char *token2) {

	int i = 0;
	while (token1[i] != '\0' && token2[i] != '\0') {
		if (toupper(token1[i]) < toupper(token2[i]))
			return -1;
		else if (toupper(token1[i]) > toupper(token2[i]))
			return 1;
		i++;
	}

	if (token1[i] == '\0' && token2[i] == '\0')
		return 0;
	else if (token1[i] == '\0')
		return -1;
	else
		return 1;

}


/*
 * Prompt the user.
 *
 * Input:
 *   buf    - a buffer into which to store the answer
 *   n      - the maximum number of characters to write to the buffer
 *   format - format string, as printf
 *   ...    - as printf
 */
void prompt_user(char *buf, int n, const char *format, ...) {

	/* print the prompt */
	va_list args;
	va_start(args, format);
	printf("%s: ", chatbot_botname());
	vprintf(format, args);
	printf(" ");
	va_end(args);
	printf("\n%s: ", chatbot_username());

	/* get the response from the user */
	fgets(buf, n, stdin);
	char *nl = strchr(buf, '\n');
	if (nl != NULL)
		*nl = '\0';
}
