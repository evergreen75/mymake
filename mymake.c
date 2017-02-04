
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
    while (!(*s == ' ' || *s == '\0') || *s == '\t' || *s == '\n')
    {
        i++;
        s++; 
    }
    return i;
}

char* getDependency(char* linePointer, int countChar)
{
    char* depend;
    depend = (char *)malloc(sizeof(char)* countChar);
    for (int i = 0; i < countChar; i++)
    {
        depend[i] = linePointer[i];
    }
    depend[countChar] = '\0';
    return depend;
}

void readLine(char* line, Rule* rule)
{
       if (strstr(line, ":") != NULL)
        {
            int i = 0;
            char target[50];
            char* g = skipSpaces(line);
            while (*g != ':')
            {
                target[i] = *g;
                g++; i++;
            }
            target[i] = '\0';
            Rule* newRule;
            addRule(rule, newRule);
            newRule->target = target;
            g++;
            g = skipSpaces(g);
            int numChars = countCharacters(g);
            int j = 0;
            while (*g != '\0')
            {
                g = skipSpaces(g);
                numChars = countCharacters(g);
                newRule->dependencies[j] = getDependency(g, numChars);
                j++;
                g = g + numChars;
            }
            printf("%s %s\n", "target is", newRule->target);
            for (int index = 0; index < j; index++)
            {
            	printf("dependency %d : %s\n", index, newRule->dependencies[i]);
            }
        }
        else if (line[0] == '\t')
        {
            //printf("command\n");
        }
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
    while (fgets(line, sizeof(line), fe))
    {
        readLine(line, rules);
        i++;
    }
   
    fclose(fe);
    return rules;
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
      readMakeFile("Makefile.txt");
      return 0;
  }