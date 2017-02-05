
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define true -1
#define false -2

typedef struct rule {
    char* target;
    char** dependencies;
    int numDependencies;
    char** commands;
    int numCommands;
    struct rule* next;
} Rule;

typedef struct linkedlist {
    struct rule* head;
} List;
// addRule(listRules, addRule) adds a Rule
// structure into a linked list of Rules

List* createList()
{
    List* hd = malloc(sizeof(*hd));
    return hd;
}

Rule* createRule(char* target, char** dependencies, int numD, char** comm, int numC, Rule* nxt)
{
    Rule* newRule = malloc(sizeof(*newRule));
    newRule->target = target;
    newRule->dependencies = dependencies;
    newRule->numDependencies = numD;
    newRule->commands = comm;
    newRule->numCommands = numC;
    newRule->next = nxt;
    return newRule;
}
void addRule(List* list, Rule* data)
{
    List* p = list;
   if (list == NULL)
   {
        list->head = data;
   }
   else
   {
        while (p->head->next != NULL)
        {
            p->head = p->head->next;
        }
        p->head->next = createRule(data->target, data->dependencies,
                            data->numDependencies, data->commands,
                            data->numCommands, data->next);
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

int isBlankLine(char* line)
{
    char* g = line;
    int length = strlen(line);
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
void readLine(char* line, List* list)
{
    int index = 0;
    if (isBlankLine(line) == true)
    {
        return;
    }
    //printf("%s %d %s %s", "line ", lineNumber, "is -> ", line);
    lineNumber++;
    char* lineCopy = line;

        // line is a target line
       if (strstr(line, ":") != NULL)
        {
            char* target = (char*)malloc(sizeof(char) * 30);
            char** dependencies = (char**)malloc(sizeof(char) * 30);
            lineCopy = skipSpaces(lineCopy);
            while (*lineCopy != ':')
            {
                target[index] = *lineCopy;
                lineCopy++; index++;
            }
            target[index] = '\0';
            index = 0;
            lineCopy+=1;
            int j = 0;
            lineCopy = skipSpaces(lineCopy);
            while (isBlankLine(lineCopy) != true)
            {
                
                lineCopy = skipSpaces(lineCopy);
                 printf("%s", lineCopy);
                 // for separating dependencies
               while (!(*lineCopy == ' ' || *lineCopy == '\t' || *lineCopy == '\n' || *lineCopy == '\0'))
                {
                    //lineCopy = skipSpaces(lineCopy);
                    //dependencies[j][1] = 'c';
                    //if (*lineCopy == '\0')
                    {
                      //  break;
                     
                    }
                    printf("%s", lineCopy);
                    lineCopy++;
                     lineCopy = skipSpaces(lineCopy);
                }
                j++;
            }
           // if (isBlankLine(lineCopy) == true)
            //{
              //  return; 
            //}
            //printf("%s", lineCopy);
        }
      
}
// readMakeFile() reads in a make file "path" and stores
// information in a linked list of Rules.

List* readMakeFile(const char* path)
{
    const int MAX_LINE_LENGTH = 100;
    int i = 0;
    char line[MAX_LINE_LENGTH];
    List* list = createList();
    FILE* fe = fopen (path, "r");
    while (fgets(line, sizeof(line), fe))
    {
        readLine(line, list);
        i++;
    }
   
    fclose(fe);
    return list;
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

      readMakeFile("Makefile2.txt");
      //g.target = "oop";
     
      //printf("%s", head->target);
     // printf("%s" , g->target);
     
      //addRule(head, newNode);

      return 0;
  }