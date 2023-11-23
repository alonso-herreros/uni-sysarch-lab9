// Initial: header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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


int main(int ac, char **av) // Initial code
{
    Node *p_first = NULL;
    Node *p_node;
    Verify_args(ac, av);

    p_first = Read(FILE_NAME);
    Display(p_first);
    p_node = New_node(av[1], av[2]);
    p_first = Add(p_first, p_node);
    Display(p_first);
    Save(p_first, FILE_NAME);
    Destroy(p_first);
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



Node *Read(const char *file_name)
{
    if (access(file_name, R_OK) == -1)  return NULL;

    Node *head = NULL;
    FILE *f = fopen(file_name, "rb");
    while (!feof(f)) {
        Node *node = (Node *) malloc(sizeof(Node));
        if (fread(node, sizeof(Node), 1, f) < 1)  continue;
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

