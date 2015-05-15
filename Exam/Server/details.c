#include "details.h"

int setListenSocket(int *listenSocket, void *address)
{
    *listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(*listenSocket < 0)
    {
        fprintf(stderr, "Error: socket.\n");
        return 1;
    }

    if(fcntl(*listenSocket, F_SETFL, O_NONBLOCK, 1) == -1)
    {
        fprintf(stderr, "Failed to set non-block mode.\n");
        close(*listenSocket);
        return 1;
    }

    int error;
    struct sockaddr_in *local = (struct sockaddr_in *)address;
    local->sin_family = AF_INET;
    local->sin_port = htons(PORT);
    local->sin_addr.s_addr = htonl(INADDR_ANY);

    error = bind(*listenSocket, (const struct sockaddr *)local, sizeof(struct sockaddr_in));
    if(error)
    {
        fprintf(stderr, "Error: bind.\n");
        close(*listenSocket);
        return 1;
    }

    error = listen(*listenSocket, LISTEN_QUEUE);
    if(error)
    {
        fprintf(stderr, "Error: listen.\n");
        close(*listenSocket);
        return 1;
    }

    return 0;
}

int addClient(Client **list, char *name, int socket)
{
    if(!strlen(name))
    {
        fprintf(stderr, "No name. Error: addClient.\n");
        return 1;
    }
    if(socket < 0)
    {
        fprintf(stderr, "Wrong socket. Error: addClient.\n");
        return 1;
    }

    Client *newClient = NULL;
    newClient = (Client *)malloc(sizeof(Client));
    if(!newClient)
    {
        fprintf(stderr, "Failed to add %s to client list.\n", name);
        return 1;
    }

    if(strlen(name) <= NAME_LENGTH)
    {
        strncpy(newClient->name, name, strlen(name));
        newClient->name[strlen(name) + 1] = '\0';
    }
    else
    {
        fprintf(stderr, "Too long name. Error: addClient.\n");
        free(newClient);
        newClient = NULL;
        return 1;
    }

    newClient->socket = socket;
    newClient->amountOfOnline = 0;
    newClient->previous = NULL;
    newClient->next = NULL;

    if(!*list)
    {
        *list = newClient;
        (*list)->amountOfOnline = 1;
    }
    else
    {
        Client *temp = *list;
        while(temp->next)
            temp = temp->next;
        temp->next = newClient;
        newClient->previous = temp;
        ++((*list)->amountOfOnline);
        temp = NULL;
    }

    newClient = NULL;

    return 0;
}

int deleteClient(Client **list, int socket)
{
    if(socket < 0)
    {
        fprintf(stderr, "Wrong socket. Error: deleteClient.\n");
        return 1;
    }
    if(!*list)
    {
        fprintf(stderr, "The client list is empty. Error: deleteClient.\n");
        return 1;
    }

    Client *temp = *list;
    while(temp && temp->socket != socket)
        temp = temp->next;

    if(temp == *list)
    {
        *list = (*list)->next;
        if(*list)
        {
            (*list)->amountOfOnline = --(temp->amountOfOnline);
            (*list)->previous = NULL;
        }
    }
    else
    {
        temp->previous->next = temp->next;
        if(temp->next)
            temp->next->previous = temp->previous;
        --((*list)->amountOfOnline);
    }

    memset(temp->name, 0, strlen(temp->name));
    temp->amountOfOnline = 0;
    temp->socket = -1;
    temp->previous = NULL;
    temp->next = NULL;

    free(temp);
    temp = NULL;

    return 0;
}

int checkTheLength(char *name)
{
    if(strlen(name) > 0 && strlen(name) < NAME_LENGTH)
        return 0;
    return 1;
}

int checkTheSame(Client *list, char *name)
{
    int returnCode = 0;
    Client *temp = list;
    while(temp)
    {
        if(!strcmp(temp->name, name))
            returnCode = 1;
        temp = temp->next;
    }
    return returnCode;
}

void writeToAllClients(Client *list, SocketInfo *info)
{
    Client *temp = list;
    while(temp)
    {
        while(write(temp->socket, info, sizeof(*info)) <= 0);
        temp = temp->next;
    }
}
