#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>


#define N_DWARVES 7
const char *DWARVES[N_DWARVES] = {"Doc", "Grumpy", "Happy", "Sleepy", "Bashful", "Sneezy", "Dopey"};


// Declarations
void Process_dwarf(pid_t ppid, const char *name);
void Process_Snowwhite();


// MAIN
int main(int argc, char **argv)
{
    pid_t ppid = getpid();
    pid_t pids[N_DWARVES];
    for (int i=0; i<N_DWARVES; i++) {
        if ((pids[i] = fork()) < 0)  exit(EXIT_FAILURE);
        else if (pids[i] == 0)  Process_dwarf(ppid, DWARVES[i]);
    }

    Process_Snowwhite();
}
