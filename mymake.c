// Author: Joshua Bowen
// Date: 02/02/16
// CSCI 3000

// The purpose of this program is to simulate a simplified version of the command make.
// Processes lines of targets/dependencies/commands given to the program in a file
// created by the user called Makefile with the standard Makefile format. 
// File must be named Makefile and contain only targets/dependencies/commands or comment lines starting
// with a '#' character.
// The target to be made must be specified in an argument when program is run. arg[1] is the argument
// of the target that is intended to be made.

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#define true 1
#define false 0

// Structure for representing a rule.

typedef struct rule {
    char* target;
    char* dependencies[10];
    int numDependencies;
    char* commands[10];
    int numCommands;
    struct rule* next;
} Rule;

void buildDependency(char* dependency, Rule* rules);
void buildTarget(char* target, Rule* rules);
int maxModTime(char* target, Rule* rules);
int upToDate(char* target, Rule* rules);

// Creates an empty Linked List from head which is a pointer
// to first Rule Structure in empty list. (NULL to start with).

void createList(Rule** head)
{
	*head = NULL;
}

// Inserts a rule into a linked list being pointed to by head. target, dependencies
// numD, comm, and numC are all variables representing their respective data fields in a rule structure.

void insert(Rule** head, char* target, char** dependencies, int numD, char** comm, int numC)
{
	Rule* newRule;
	Rule* temp;
	newRule = (Rule*)malloc(sizeof(Rule));
	newRule->next = NULL;
	newRule->target = malloc(sizeof(char)* 30);
	strcpy(newRule->target, target);
	int i;
	for (i = 0; i < numD; i++)
	{
		newRule->dependencies[i] = (char*)malloc(sizeof(char) * 30);
		strcpy(newRule->dependencies[i], dependencies[i]);
	}	
	newRule->numDependencies = numD;
	for (i = 0; i < numC; i++)
	{
		newRule->commands[i] = (char*)malloc(sizeof(char) * 30);
		strcpy(newRule->commands[i], comm[i]);
	}
	// ** Since we are inserting commands into a rule AFTER it has been intialized already. **	
	if (numC == 0)
	{
		for (i = 0; i < 10; i++)
		{
			newRule->commands[i] = (char*)malloc(sizeof(char) * 30);
		}
	}  // ****************************************************************************
    newRule->numCommands = numC;
    if (*head == NULL)
    {
    	*head = newRule;
    }
    else
    {
    	temp = *head;
		*head = newRule;
		newRule->next = temp;
    }
}

// Prints a list of rules pointed to by head.

void printListOfRules(Rule* head)
{
	Rule* g;
	for (g = head; g!= NULL; g = g->next)
	{
		int i;
		printf("%s %s\n", "Target: ", g->target);
		for (i = 0; i < g->numDependencies; i++)
		{
			printf("%s %d %s %s \n", "Dependency", i, "= ", g->dependencies[i]);
		}
		printf("%s %d \n", "Number of dependencies is:", g->numDependencies);
		for (i = 0; i < g->numCommands; i++)
		{
			printf("%s %d %s %s \n", "Command", i, "= ", g->commands[i]);
		}
		printf("%s %d \n", "Number of commands is:", g->numCommands);
		printf("\n");
	}
}

// Prints an array of strings of type char**. i is used 
// to indicate size of the pointer array.

void printArrayOfStrings(char* arrayString[], int i)
{
	int j;
    for (j = 0; j < i; j++)
    {
        printf("%d %s %s  ", j,":",  arrayString[j]);
    }
	printf("\n");
}

// Skips spaces in a string and returns a pointer to the first non-whitespace
// character available.

char* skipSpaces(char* str)
{
    char* s = str;
    while (*s == '\t' || *s == ' ' || *s == '\n')
    {
        s++;
    }
    return s;
}

// Checks whether or not a line is blank with all characters
// being whitespace. Returns true if so, false if not.

int isBlankLine(char* line)
{
    char* g = line;
    int length = strlen(g);
    int index = 0;
    while (index < length)
    {
        if (*g == '\0' || *g == '\n')
        {
            return true;
        }
        else if (*g != '\t' && *g != ' ')
        {
            return false;
        }
        index++;
        g++;
    }
    return true;
}

// Processes a line, and stores individual strings into array of strings
// lineType. Can be used for processing a line of dependencies or commands.
// Returns number of individual strings read into lineType.

int processLine(char* line, char** lineType)
{
	char buffer[50];
	int stringNumber = 0;
	int charIndex = 0;
	char* linePtr = line;
	linePtr = skipSpaces(linePtr);
		while (isBlankLine(linePtr) != true)
		{
			linePtr = skipSpaces(linePtr);
			while (!isspace(*linePtr))
			{
				if (*linePtr == '\0')
				{
					buffer[charIndex] = *linePtr;
					strcpy(lineType[stringNumber], buffer);
					break; 
				}
				else
				{
					buffer[charIndex] = *linePtr;
					linePtr++; charIndex++;
				}
			}
			buffer[charIndex] = '\0';
			charIndex = 0;
			strcpy(lineType[stringNumber], buffer);
			stringNumber++;
		}
		return stringNumber;
}

// Reads in a line and stores rules found into a linked list pointed
// to by head. 

void readLine(char* line, Rule** head)
{
	char* linePtr = line;
	// Line is a target line check. Process target first before proceeding.
	// Create a new rule as well.
	if (strstr(line, ":") != NULL)
	{
		int i;
		char** dependencies = (char**)malloc(sizeof(char*) * 10);
		for (i = 0; i < 10; i++)
		{
			dependencies[i] = (char*)malloc(sizeof(char) * 30);
		}
		char* target = (char*)malloc(sizeof(char) * 30);
		linePtr = skipSpaces(linePtr);
		int index = 0;
		while (*linePtr != ':')
		{
			target[index] = *linePtr;
			index++; linePtr++;
		}
		target[index] = '\0';
		linePtr++;
		int numDependencies = processLine(linePtr, dependencies);
		insert(head, target, dependencies, numDependencies, NULL, 0);
	}
	// Line is a command line check. Process commands into already created rule
	// from previous target.
	else if (line[0] == '\t')
	{
		if ((*head)->numCommands >= 10)
		{
			printf("%s%s", "Error. Maximum number of commands exceeded for target ", (*head)->target);
		}
		strcpy((*head)->commands[(*head)->numCommands], line);
		(*head)->numCommands++;
	}
	// Line is a comment line.
	else if (line[0] == '#')
	{
		return;
	}  
}

// Reads in a make file "path" and stores
// information in a linked list of Rules.
// Returns this linked list of Rules.

Rule* readMakeFile(const char* path)
{
    const int MAX_LINE_LENGTH = 100;
    char line[MAX_LINE_LENGTH];
   	Rule* head = malloc(sizeof(Rule));
   	createList(&head);
    FILE* fe = fopen (path, "r");
	if (fe == NULL)
	{
		printf("ERROR. The make file %s could not be found \n", path);
		exit(1);
	}
    while (fgets(line, sizeof(line), fe))
    {
        readLine(line, &head);
    }
    fclose(fe);
    return head;
}

// Finds a rule for a given target in a linked
// list of rules pointed to by list.

Rule* findRule(const char* target, Rule* list)
{
    for (Rule* p = list; p!= NULL; p = p->next)
    {
        if (strcmp(target, p->target) == 0)
        {
            return p;
        }
    }
    return NULL;
}

// runCommand(argv) runs a command given
// by a string "command".

void runCommand(char* command)
{
	char** argv = malloc(sizeof(char*) * 20);
	int i;
	for (i = 0; i < 20; i++)
	{
		argv[i] = malloc(sizeof(char) * 30);
	}
	int numCommands = processLine(command, argv);
	argv[numCommands + 1] = NULL;
    pid_t child = fork();
    int status;
    if (child > 0)
    {
		pid_t par;
        par = wait(&status);
    }
    else
    {
		execvp(argv[0], argv);
		printf("\n%s\n", "error");
		exit(0);
    } 
}

// Checks whether or not a file "file" exists.
// Returns true if it does, and false if not.

int fileExists(const char* file)
{
	struct stat  buffer;   
  	return (stat (file, &buffer) == 0);
}

// Builds a single dependency "dependency" 
// using a rule from rules.

void buildDependency(char* dependency, Rule* rules)
{
	if (findRule(dependency, rules) != NULL)
	{
		buildTarget(dependency, rules);
	}
	else if (fileExists(dependency))
	{
		return;
	}

	else
	{
		printf("Error, dependency %s could not be built\n", dependency);
	}
}

// Builds a target given by "target", using a rule from
// rules.

void buildTarget(char* target, Rule* rules)
{
	int outOfDate = -1;
	int i;
	Rule* g = findRule(target, rules);
	if (g == NULL)
	{
		printf("ERROR. Target %s does not exist in the Makefile \n", target);
		exit(1);
	}
	for (i = 0; i < g->numDependencies; i++)
	{
		buildDependency(g->dependencies[i], rules);
	}
	if (!fileExists(target))
	{
		outOfDate = true;
	}
	for (i = 0; i < g->numDependencies; i++)
	{
		if (!fileExists(g->dependencies[i]))
		{
			outOfDate = true;
			break;
		}
	}
	if (!upToDate(target, rules))
	{
		outOfDate = true;
	}
	if (outOfDate == true)
	{
		for (i = 0; i < g->numCommands; i++)
		{
			printf("Running command.. %s", g->commands[i]);
			runCommand(g->commands[i]);
		}
	}
	else if (outOfDate != true)
	{
		printf("Target %s is up to date\n", target);
	}
}

// Returns max of two numbers

int max(int x, int y)
{
	if (x > y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

// Checks whether or not a target is up to date
// Returns true if so, and false if not.

int upToDate(char* target, Rule* rules)
{
	struct stat st2;
	int modTime = 0;
	if (stat(target, &st2) == 0)
	{
		modTime = st2.st_mtime;
	}
	return modTime >= maxModTime(target, rules);
}

// Calculates maximum modification time of 
// a target's dependencies in use as a helper function.

int maxModTime(char* target, Rule* rules)
{
	Rule* rl = findRule(target, rules);
	int maxMod = 0;
	struct stat st;
	int i;
	for (i = 0; i < rl->numDependencies; i++)
	{
		if (stat(rl->dependencies[i], &st) == 0)
		{
			maxMod = max(maxMod, st.st_mtime);
		}
	}
	return maxMod;
}

int main(int argc, char** argv)
{
	Rule* head;
  	createList(&head);
  	head = readMakeFile("Makefile");
	if (argc == 2)
	{
		buildTarget(argv[1], head);
	}
	else
	{
		printf("%s\n", "Error, invalid number of command line arguments.");
	}
	return 0;
}