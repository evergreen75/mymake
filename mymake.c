
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Rule {

} Rule;

// readMakeFile() reads in a make file "path" and stores
// information in a linked list of Rules.

Rule* readMakeFile(const char* path)
{
    FILE* fe = fopen (path, "r");
    fclose(fe);
    return NULL;
}

// parseCommand(target, rules) finds a rule for a target
// and stores the command for building into an array of strings.

char** parseCommand(char* target, Rule* rules)
{
    return NULL;
}

// runCommand(argv) runs a command given by an array
// of strings.

void runCommand(char** argv)
{

}
int main(void)
  {
      
  }