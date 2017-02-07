
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define true -1
#define false -2

typedef struct rule {
    char* target;
    char* dependencies[10];
    int numDependencies;
    char* commands[10];
    int numCommands;
    struct rule* next;
} Rule;

void createList(Rule** head)
{
	*head = NULL;
}

void insertRule(Rule** head, Rule* newRule)
{
	Rule* current;
	if (*head == NULL)
	{
		*head = newRule;
	}
	  else
    {
    	current = *head;
    	while (current->next != NULL)
    	{
    		current = current->next;
    	}
    	current->next = newRule;
    	newRule->next = NULL;
    }
}

void fillRule(Rule* newRule, char* target, char** dependencies, int numD, char** comm, int numC)
{
	newRule->next = NULL;
	newRule->target = target;
	for (int i = 0; i < numD; i++)
	{
		newRule->dependencies[i] = dependencies[i];
	}	
	newRule->numDependencies = numD;
    for (int i = 0; i < numC; i++)
	{
		newRule->commands[i] = comm[i];
	}	
    newRule->numCommands = numC;
}
void insert(Rule** head, char* target, char** dependencies, int numD, char** comm, int numC)
{
	Rule* newRule;
	Rule* current;
	newRule = (Rule*)malloc(sizeof(Rule));
	newRule->next = NULL;
	newRule->target = target;
	for (int i = 0; i < numD; i++)
	{
		newRule->dependencies[i] = dependencies[i];
	}	
	newRule->numDependencies = numD;
	for (int i = 0; i < numC; i++)
	{
		newRule->commands[i] = comm[i];
	}	
    newRule->numCommands = numC;
    if (*head == NULL)
    {
    	*head = newRule;
    }
    else
    {
    	current = *head;
    	while (current->next != NULL)
    	{
    		current = current->next;
    	}
    	current->next = newRule;
    }
}


void printArrayOfStrings(char* arrayString[], int i)
{
    for (int j = 0; j < i; j++)
    {
        printf("%s\n", arrayString[j]);
    }
}

char* skipSpaces(char* str)
{
    char* s = str;
    while (*s == '\t' || *s == ' ' || *s == '\n')
    {
        s++;
    }
    return s;
}

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


void storeDependency(char* dep, char* line)
{
    int i = 0;
    char* cpy = line;
    while (*line != ' ' && *line != '\n' && *line != '\t' && *line != '\0')
    {
        dep[i] = *line;
        line++;
        i++;
    }
}


int lineNumber = 1;
void readLine(char* line, Rule* list)
{
   // printf("%s %d %s %s", "line ", lineNumber, "is -> ", line);
	if (strstr(line, ":") != NULL)
	{
		char** dependencies = (char**)malloc(sizeof(char*) * 10);
		for (int i = 0; i < 10; i++)
		{
			dependencies[i] = (char*)malloc(sizeof(char) * 30);
		}
		char* target = (char*)malloc(sizeof(char) * 30);
		char* linePtr = line;
		size_t ln = strlen(linePtr) - 1;
		if (line[ln] == '\n')
		{
			line[ln] = '\0';
		}
		linePtr = skipSpaces(linePtr);
		int index = 0;
		while (*linePtr != ':')
		{
			target[index] = *linePtr;
			index++; linePtr++;
		}
		target[index] = '\0';
		linePtr++;
		linePtr = skipSpaces(linePtr);
		if (isBlankLine(linePtr) == true)
		{
			return;
		}
		
		int stringNumber = 0;
		int charIndex = 0;
		int numDependencies = 0;
		while (isBlankLine(linePtr) == false)//(isBlankLine(linePtr) == false)
		{
			char buffer[50]; //= (char*)malloc(sizeof(char) * 50);
			linePtr = skipSpaces(linePtr);
			while (!isspace(*linePtr))
			{
				// if last string [hello] error handle null char not added**** 
				if (*linePtr == '\0')
				{
					buffer[charIndex] = *linePtr;
					strcpy(dependencies[stringNumber], buffer);
					break; 
				}
				else
				{
					buffer[charIndex] = *linePtr;
					//printf("%c %c\n", buffer[charIndex], *linePtr);
					linePtr++; charIndex++;
				}
			}
			buffer[charIndex] = '\0';
			charIndex = 0;
			//printf("%s", buffer);
			strcpy(dependencies[stringNumber], buffer);
			//free(buffer);
			stringNumber++;

			//printf("%d", stringNumber);
		}
		int a = 0;
		//strcpy(dependencies[0], "THIS IS A TEST");
		printArrayOfStrings(dependencies, stringNumber);
		while (a < stringNumber)
		{
			//printf("%s\n", dependencies[a]);
			a++;
		}

	}
}

// readMakeFile() reads in a make file "path" and stores
// information in a linked list of Rules.

Rule* readMakeFile(const char* path)
{
    const int MAX_LINE_LENGTH = 100;
    int i = 0;
    char line[MAX_LINE_LENGTH];
   	Rule* head;
   	createList(&head);
    FILE* fe = fopen (path, "r");
    while (fgets(line, sizeof(line), fe))
    {
        readLine(line, head);
        i++;
    }
   for (Rule* p = head; p!= NULL; p = p->next)
   {
   	printf("%s", p->dependencies[0]);
   }
    fclose(fe);
    return head;
}

// parseCommand(target, rules) finds a rule for a target
// and stores the command for building into an array of strings.

char** parseCommand(const char* target, Rule* rules)
{
    return NULL;
}

// runCommand(argv) runs a command given by an array
// of strings.

void runCommand(char** argv)
{

}

Rule* findRule(const char* target)
{
    return NULL;
}
int main(void)
  {
  	Rule* head;
  	createList(&head);
  	readMakeFile("Makefile2.txt");
  	Rule* rl = (Rule*)malloc(sizeof(Rule));
  	//rl->target = "hello";
  	Rule* g = (Rule*)malloc(sizeof(Rule));
  	rl->next = NULL;
  	fillRule(rl, "hello", NULL, 0, NULL, 0);
  	g->next = NULL;
  	g->target = "chase";
  	insertRule(&head, rl);
  	insertRule(&head, g);
  	
  	//insert(&head, "test2", NULL, 0, NULL, 0);
  
    
      return 0;
  }