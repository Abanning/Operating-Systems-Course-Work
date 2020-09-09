/* @filename: simple-shell.c
 * @author: Alex Banning (collaborated on logic with Nikki Creamer)
 * @assignment: Project 3 - Simple Shell Program
 * @date: October 23rd, 2019
 * @description: This program runs a basic Linux shell and keeps track
 * 				 of the history of the commands entered.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#define MAX_LINE		80 		/* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	9 	/* size of history */

char history[MAX_COMMANDS][MAX_LINE]; //the array used to store history commands.
char display_history[MAX_COMMANDS][MAX_LINE]; 
/*the array used for "printf" to display history nicely. Remove special characters like "\n" or "\0"*/

int command_count = 0;

/* Determines if a character is the ASCII value of a digit
 *
 */
int isDigit(char character) {
	if ((character >= '0') & (character <= '9')) {
		//printf("Is a digit.\n");
		return 1;
	}
	else
		return 0;
}

/* Determines if the character is an exclaimation mark
 *
 */
int isExclaimation(char character) {        
	if (character == '!') {
		return 1;
	}
	else {
		return 0;
	}
}

/* Checks if the character is alphanumeric
 *
 */
int isAlphanumeric(char character) {
	if ( (character >= 'a' & character <= 'z') | (character >= 'A' | character <= 'Z') | isDigit(character) ) {
		return 1;
	}
	
	return 0;
}

/* Sanitizes the character input, and returns a safe one
 *
 */
char sanitizeCharacter(char dirtyChar) {
	if (isAlphanumeric(dirtyChar) == 1) {
		return dirtyChar;
	}
	else if ( (dirtyChar == '\0') | (dirtyChar == '\n') ) {
		return (' ');
	}
	else {
		return (' ');
	}
}

/*
 * Add the most recent command to the history.
 */
void addtohistory(char inputBuffer[], int length) {
	char displayChar;
	int historyIndex = ((command_count - 1) % MAX_COMMANDS);

	/* For testing purposes
	printf("command count: %d\n", command_count);
	printf("History index: %d\n", historyIndex);
	*/
	for (int i = 0; i < length; i++) {
		history[historyIndex][i] = inputBuffer[i];
		displayChar = sanitizeCharacter(inputBuffer[i]);
		display_history[historyIndex][i] = displayChar;
	}

	return;
}

/*
 * Display the recent history
 */
void displayHistory(void) {
	int displayIndex = ((command_count - 1) % MAX_COMMANDS);
	int count;
	int bigIndex;
	int min = 1;
	int flag = 0;
	int newIndex = 0;

	// Print init
	printf("History\n");
	printf("Index\t | \tCommand\n");
	// Small number of commands
	if (command_count <= 9) {
		count = command_count % MAX_COMMANDS;
		if (count == 0) {
			count = 10;
			min = 2;
			flag = 1;
		}
		for (int i = displayIndex; count >= min; i--) {
			if (flag) {
				printf("%d:\t \t", count - 1);
			}
			else {
				printf("%d:\t \t", count);
			}
			for (int j = 0; display_history[i % MAX_COMMANDS][j]; j++) {
				printf("%c", display_history[i % MAX_COMMANDS][j]);
			}
			count--;
		}
	}
	// Large number of commands
	else {
		count = 9;
		for (int i = displayIndex; count >= 1; i--) {
			printf("%d:\t \t", count);
			if (i < 0) {
				newIndex = i + MAX_COMMANDS;
			}
			else {
				newIndex = i;
			}
			for (int j = 0; display_history[newIndex][j]; j++) {
				printf("%c", display_history[newIndex][j]);
			}
			count--;
		}
	}
	
}

int charToInt(char character) {
	return (character - '0');
}

int getLength(char *argument) {
	int count = 0;
	int shouldKeepGoing = 1;

	while(shouldKeepGoing) {
		if (argument[count] == '\0' | argument[count] == '\n') {
			shouldKeepGoing = 0; // stop while condition
		}
		count++; // increment count
	}

	return count;
}

/** 
 * The setup function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */
int setup(char inputBuffer[], char *args[], int *background) {
	// Declare and initialize local variables -------------------------------------------
	int length,					/* # of characters in the command line */
	i,							/* loop index for accessing inputBuffer array */
	command_number,				/* index of requested command number */
	count,						/* variable length for inputBuffer */
	arg_count = 0;				/* number of arguments in char *arg[] */
	int shouldRunInBackG = 0; 	/* Local flag to point background to */
	int isNewArg = 1;			/* Local flag for arg separation */
	int isADigit = 0;			/* Local flag for digit recognition */
	int isAnExclaimation = 0;	/* Local flag for exclaimation recog. */
	int updatedLength = 0;		/* Local variable for updating length with history requests */
	// ----------------------------------------------------------------------------------
	
	
	
    // Read input -----------------------------------------------------------------------
	do {
		printf("osh > ");
		fflush(stdout);
		length = read(STDIN_FILENO, inputBuffer, MAX_LINE); 
	}
	while (inputBuffer[0] == '\n'); /* swallow newline characters */

	if (length == 0)
		exit(0);            /* ^d was entered, end of user command stream */
 
	if (length < 0) {
		perror("error reading the command");
		printf("Goodbye.\n");
		printf("\n");
		exit(-1);           /* terminate with error code of -1 */
	}
	// ----------------------------------------------------------------------------------
	
	//printf("Length: %d\n", length);
	// For testing purposes
	/*printf("\n");
	printf("Original input buffer: ");
	for (int i = 0; i < length; i++) {
		if (inputBuffer[i] == '\n')
			printf("*newline*");
		else
			printf("%c", inputBuffer[i]);
	}
	printf("\n");
	printf("Length: %d\n", length);
	printf("\n");
	printf("\n");
	*/
	
	command_count++; // increment command count
	
	// Check for history operation ------------------------------------------------------
	if (length >= 3) { // '!','X','\n'
		isAnExclaimation = isExclaimation(inputBuffer[0]); // check first item
		if (isAnExclaimation) { // if firstItem == '!', then keep going
			isADigit = isDigit(inputBuffer[1]); // check second item for 'X'
			isAnExclaimation = isExclaimation(inputBuffer[1]); // check second item for '!'
			if (isADigit) { // If secondItem == 'X', then choose history
				command_number = charToInt(inputBuffer[1]) - 1;
				if (command_number >= command_count) {
					printf("No such command in history!\n");
					perror("no such command in history");
					exit(-1);
				}
				updatedLength = getLength(&history[command_number][0]);
				/* For testing
				printf("updated length: %d\n", updatedLength);
				printf("Copying command #: %d\n", command_number + 1);
				printf("Copied command: ");
				*/
				for (int i = 0; i < updatedLength; i++) {
					inputBuffer[i] = history[command_number][i];
					/* For testing
					if (inputBuffer[i] == '\0') {
						printf(" ");
					}
					else {
						printf("%c", inputBuffer[i]);
					}
					printf("!");
					*/
				}
				// printf("\n");
			}
			else if (isAnExclaimation) {
				if (command_count == 0) {
					printf("No commands have been entered yet!\n");
					perror("no commands have been entered");
					exit(-1);
				}
				command_number = (command_count - 1) % MAX_COMMANDS; // most recent command
				/* For testing purposes
				printf("Command number: %d\n", command_number);
				*/
				printf("Most recent command: ");
				updatedLength = getLength(&history[command_number - 1][0]);
				for (int i = 0; i < updatedLength; i++) {
					printf("%c", history[command_number - 1][i]);
					inputBuffer[i] = history[command_number - 1][i];
				}
			}
			else {
				perror("error reading the command");
				printf("Goodbye.\n");
				printf("\n");
				exit(-1);
			}
		}
	}
	// ----------------------------------------------------------------------------------
	
	// Add to history
	updatedLength = getLength(&inputBuffer[0]);
	/* For testing purposes
	printf("Length: %d\n", length);
	printf("updatedLength: %d\n", updatedLength);
	*/
	addtohistory(inputBuffer, updatedLength);
	// ----------------------------------------------------------------------------------



	// Parse the contents of inputBuffer ------------------------------------------------
	for (i = 0; i < updatedLength; i++) { 
		/* examine every character in the inputBuffer */
		switch (inputBuffer[i]) {
			case ' ': // fall-through case
			case '\t': /* argument separators */
			case '\0':
				inputBuffer[i] = '\0'; // change inputBuffer char
				isNewArg = 1; // set flag
				break;
			case '\n':  /* should be the final char examined */
				inputBuffer[i] = '\0';
				args[arg_count] = NULL; // set final arg to NULL
				isNewArg = 0; // no more new args
				break;
			case '&':
				shouldRunInBackG = 1; // set local background flag
				break;
			default : /* some other character */
				if (isNewArg) { // we only care if it's not part of the string
					args[arg_count] = &inputBuffer[i]; // set arg pointer
					arg_count++; // increment arg count
					isNewArg = 0; // reset flag
				}
		} /* end of switch */
	}    /* end of for */
	// ----------------------------------------------------------------------------------


	// For testing purposes
	/*
	printf ("Address \t | \t Value\n");
	for (int i = 0; i < length; i++) {
		if (inputBuffer[i] == '\0') {
			printf("%p \t | \t ", &inputBuffer[i]);
			printf("*NULL*\n");
		}
		else {
			printf("%p \t | \t %c\n", &inputBuffer[i], inputBuffer[i]);
		}
	}
	printf("Length: %d\n", length);
	*/

	// Set background flag
	background = &shouldRunInBackG;

	// Check for 'history' or 'exit' strings --------------------------------------------
	if ((strncmp(&inputBuffer[0], "history", length) == 0)) {
		displayHistory();
		return 2; // special case code for history
	}
	else if ((strncmp(&inputBuffer[0], "exit", length) == 0)) {
		printf("Goodbye.\n");
		exit(0);
	}
	// ----------------------------------------------------------------------------------

	// Return 1 for normal operation
	return 1;
	
} /* end of setup routine */


// For testing purposes -----------------------------------------------------------------
/*
int mystrncmp(char* string1, const char* string2, int length) {
	int flag = 0;
	for (int i = 0; (i < length) & (string1[i] != '\n'); i++) {
		if (string1[i] == string2[i]) {
			printf("Char1: %c\t", string1[i]);
			printf("Char2: %c\n", string2[i]);
		}
		else {
			flag = 1;
			printf("Char1: %c\t", string1[i]);
			printf("Char2: %c\n", string2[i]);
			return flag;
		}
	}

	return flag;
}
*/
// --------------------------------------------------------------------------------------


/* Main function - runs shell program
 *
 */
int main(void)
{
	// Declare and initialize local variables -------------------------------------------
	char inputBuffer[MAX_LINE]; 	/* buffer to hold the command entered */
	int background;             	/* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/2 + 1];		/* command line (of 80) has max of 40 arguments */
	pid_t pid, child;		        /* process id of the parent process */
	int shouldrun = 1;				/* Local flag for while continuation */
	int waitStatus;					/* Local variable for status of child process */
	// ----------------------------------------------------------------------------------
	
	
	
	// Run Shell ------------------------------------------------------------------------
	while (shouldrun) {            		/* Program terminates normally inside setup */
		background = 0; // by default, foreground process
		
		// Run setup
		shouldrun = setup(inputBuffer, args, &background);       /* get next command */
		//printf("Shouldrun? %d\n", shouldrun); // for testing
		
		if (shouldrun == 1) {
			pid = fork();
			if (pid == -1) {
				perror("error creating duplicate process");
				printf("Goodbye.\n");
				printf("\n");
				exit(-1);
			}
			else if (pid == 0) { // child process created
				// For testing purposes
				//printf("Child process created.\n");
				//printf("Executing exec process\n");
				execvp(args[0], args);
				//printf("Back from exec process.\n");
				//printf("\n");
				return 0;
			}
	
			// Check for foreground process
			if (background == 0) {
				child = wait(&waitStatus); // wait for child to terminate
			}
		}
		else if (shouldrun == 2) {
			//printf("Should be displaying history rn...\n");
			shouldrun = 1;
		}
		// Else Exit
		else {
			printf("Goodbye.\n");
			exit(0);
		}
    }
	// ----------------------------------------------------------------------------------
	return 0;
}
