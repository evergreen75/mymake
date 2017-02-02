
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct rule {
    char* target;
    char* dependencies[10];
    int numDependencies;
    char* commands[10];
    int numCommands;
    struct rule* next;
} Rule;

// addRule(listRules, addRule) adds a Rule
// structure into a linked list of Rules.

void addRule(Rule* listRules, Rule* addRule)
{
    if (listRules == NULL)
    {
        listRules = (Rule*) malloc (sizeof (Rule));
    }
    else
    {
        for (Rule* p = listRules; p->next != NULL; p = p->next)
        {
            if (p->next == NULL)
            {
                p->next = addRule;
            }
        }
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
    while (*s == '\t' || *s == ' ')
    {
        s++;
    }
    return s;
}

int countCharacters(char* str)
{
    char* s = str;
    int i = 0;
    while (*s != ' ' || *s != '\t')
    {
        i++;
        s++;
    }
    return i;
}

// readMakeFile() reads in a make file "path" and stores
// information in a linked list of Rules.

Rule* readMakeFile(const char* path)
{
    const int MAX_LINE_LENGTH = 100;
    int i = 0;
    char line[MAX_LINE_LENGTH];
    Rule* rules = (Rule*) malloc (sizeof (Rule));
    FILE* fe = fopen (path, "r");
    while (fgets(line, sizeof(line), fe)){
        if (strstr(line, ":") != NULL)
        {
            //printf("target\n");
        }
        else if (line[0] == '\t')
        {
            //printf("command\n");
        }
        else
        {

        }
      i++;
    }
   
    fclose(fe);
    return NULL;
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
      readMakeFile("Makefile");
      return 0;
  }