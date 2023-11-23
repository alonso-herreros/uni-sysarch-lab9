// Initial: header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define IP_SIZE 20
#define MAC_SIZE 20
#define TRACE printf("%s:%d\n", __FILE__, __LINE__)

#define FILE_NAME "tmp/Nodes.dat"

typedef struct Node // Initial code
{
    char ip[IP_SIZE];
    char mac[MAC_SIZE];
    struct Node *next;
} Node;


// Initial: declarations
void Verify_args(int ac, char **av);

Node *New_node(const char *ip, const char *mac);
Node *Add(Node *p_first, Node *p_node);

Node *Read(const char *file_name);
void Save(Node *p_first, const char *file_name);

void Display(Node *p_first);
void Destroy(Node *p_first);

void Parent_handler(int sig);

void Child_process();
void Child_handler(int sig);


static volatile int keep_alive = 1;
static volatile int displayed = 0;


int main(int ac, char **av) // Initial code
{
    Verify_args(ac, av);

    signal(SIGUSR1, Parent_handler);

    pid_t pid;
    if ((pid = fork()) == -1)  exit(EXIT_FAILURE);
    else if (pid == 0)  Child_process();
    sleep(1);

    displayed = 0;
    kill(pid, SIGUSR1);
    while(!displayed);

    Node *p_first = Read(FILE_NAME);
    p_first = Add(p_first, New_node(av[1], av[2]));
    Save(p_first, FILE_NAME);
    Destroy(p_first);

    displayed = 0;
    kill(pid, SIGUSR1);
    while(!displayed);

    kill(pid, SIGINT);
    int status;
    pid = wait(&status);
    if (status)  {
        fprintf(stderr, "Process %d terminated with error: %d\n", pid, status);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}


void Verify_args(int ac, char **av) // Initial code
{
    if (ac != 3)
    {
        printf("usage: %s <ip> <mac>\n", av[0]);
        exit(EXIT_SUCCESS);
    }
}


Node *New_node(const char *ip, const char *mac) // Initial code
{
    Node *p_node = (Node *) malloc(sizeof(Node));
    strncpy(p_node->ip, ip, IP_SIZE);
    strncpy(p_node->mac, mac, MAC_SIZE);
    p_node->next = NULL;
    return (p_node);
}

Node *Add(Node *p_first, Node *p_node)
{
    if (p_first == NULL)  return p_node; // Empty list case

    Node *curr;
    for (curr = p_first; curr->next != NULL; curr = curr->next);
    curr->next = p_node;
    return p_first;
}


Node *Read(const char *file_name)
{
    if (access(file_name, R_OK) == -1)  return NULL;

    Node *head = NULL;
    FILE *f = fopen(file_name, "rb");
    while (!feof(f)) {
        Node *node = (Node *) malloc(sizeof(Node));
        if (fread(node, sizeof(Node), 1, f) < 1) {
            free(node);
            continue;
        }
        node->next = NULL;
        head = Add(head, node);
    }
    fclose(f);
    return head;
}

void Save(Node *p_first, const char *file_name) // Initial code
{
    FILE *f = fopen(file_name, "wb");
    if (p_first != NULL)
    {
        for (Node *p = p_first; p; p = p->next)
            fwrite(p, sizeof(Node), 1, f);
    }
    fclose(f);
}


void Display(Node *p_first) // Initial code
{
    puts("--------Node list----------");
    if (p_first == NULL)
        puts("Empty");
    else
    {
        for (Node *p = p_first; p; p = p->next)
            printf("ip=\"%s\" mac=\"%s\" \n", p->ip, p->mac);
    }
    puts("-------------------------");
}

void Destroy(Node *p_first)
{
    Node *curr = p_first;
    Node *next = NULL;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}


void Child_process() {
    signal(SIGUSR1, Child_handler);
    signal(SIGINT, Child_handler);
    while (keep_alive);
    exit(EXIT_SUCCESS);
}


void Parent_handler(int sig) {
    if (sig == SIGUSR1) {
        displayed = 1;
    }
}


void Child_handler(int sig) {
    switch (sig) {
        case SIGUSR1: {
            Node *p_first = Read(FILE_NAME);
            Display(p_first);
            Destroy(p_first);
            kill(getppid(), SIGUSR1);
            break;
        }
        case SIGINT:{
            keep_alive = 0; // Don't want to interrupt other things
        }
    }
}
