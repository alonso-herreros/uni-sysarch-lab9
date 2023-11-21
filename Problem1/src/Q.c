#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void Manejador(int s);
void Proceso_hijo();

int fin;
pid_t pid;

int main(int ac, char **av)
{
    int status;

    fin = 0;

    printf("I am father (%d)\n", getpid());

    signal(SIGUSR1, Manejador);
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Error en el fork\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
        Proceso_hijo();

    while (!fin);

    pid = wait(&status);

    printf("%d has finished\n", pid);
    exit(EXIT_SUCCESS);
}

void Manejador(int s)
{
    fin = 1;
    puts("I have received a signal");
}