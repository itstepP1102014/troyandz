#include "Akinator.h"

void addNewFork(Node *oldAnimal, Node *newAnimal, Node *question, bool answer)
{
    if(answer)
    {
        question->yeslink = newAnimal;
        question->nolink = oldAnimal;
    }
    else
    {
        question->yeslink = oldAnimal;
        question->nolink = newAnimal;
    }
    oldAnimal->parent = question;
    newAnimal->parent = question;
}

void hangFork(Node **root, Node *parent, Node *fork, bool wayToChild)
{
    if(!parent)
        *root = fork;
    else
    {
        if(wayToChild)
            parent->yeslink = fork;
        else
            parent->nolink = fork;
    }
}

bool determineWayToChild(Node *node)
{
    if(!node || !node->parent)
        return false;
    else
        return node->parent->yeslink == node;
}

bool addNewKnowledge(Node **root, Node *destination, char *question, char *newAnimal, bool rightAnswer)
{
    assert(destination);
    Node *questionNode = NULL, *newAnimalNode = NULL;
    if(!createNewNode(&questionNode, question, noanimal))
        return false;
    if(!createNewNode(&newAnimalNode, newAnimal, animal))
    {
        free(questionNode);
        questionNode = NULL;
        return false;
    }
    Node *parent = destination->parent;
    bool wayToChild = determineWayToChild(destination);
    addNewFork(destination, newAnimalNode, questionNode, rightAnswer);
    hangFork(root, parent, questionNode, wayToChild);
    return true;
}

bool createNewNode(Node **node, char *str, Type type)
{
    char *p = NULL;
    p = (char *)malloc((strlen(str) + 1) * sizeof(char));
    if(!p)
        return false;
    Node *q = NULL;
    q = (Node *)malloc(sizeof(Node));
    if(!q)
    {
        free(p);
        p = NULL;
        return false;
    }
    strcpy(p, str);
    q->data.str = p;
    q->data.sizestr = strlen(p) + 1;
    q->data.type = type;
    *node = q;
    p = NULL;
    q = NULL;
    return true;
}

Node *playGame(Node *root)
{
    Node *p = root;
    while(p->data.type == noanimal)
    {
        printf("%s\n", p->data.str);
        printf("Нажми y/n: ");
        char answer;
        scanf(" %c", &answer);
        if(answer == 'y')
            p = p->yeslink;
        else
            p = p->nolink;
    }
    return p;
}

// -----

bool addNode(Node **root, Node **parent, Data data, bool way)
{
    Node *temp = NULL;
    temp = (Node *)malloc(sizeof(Node));
    if(!temp)
        return false;
    temp->data.str = (char *)malloc(data.sizestr * sizeof(char));
    if(!temp->data.str)
        return false;
    strcpy(temp->data.str, data.str);
    temp->data.sizestr = data.sizestr;
    temp->data.type = data.type;
    temp->yeslink = NULL;
    temp->nolink = NULL;
    if(!*root)
    {
        *root = temp;
        temp->parent = NULL;
    }
    else
    {
        if(way)
            (*parent)->yeslink = temp;
        else
            (*parent)->nolink = temp;
        temp->parent = *parent;
        *parent = temp;
    }
    temp = NULL;
    return true;
}

bool AddNode2(Node **root, Node **node, int way, char *str, Type type)
{
    char *p = NULL;
    p = (char *)malloc((strlen(str) + 1) * sizeof(char));
    if(!p)
        return false;
    Node *q = NULL;
    q = (Node *)malloc(sizeof(Node));
    if(!q)
    {
        free(p);
        p = NULL;
        return false;
    }
    strcpy(p, str);
    q->data.str = p;
    q->data.sizestr = strlen(p) + 1;
    q->data.type = type;
    if(!*root)
    {
        *root = q;
        (*root)->parent = NULL;
        (*root)->yeslink = NULL;
        (*root)->nolink = NULL;
    }
    else
    {
        if(way)
            (*node)->nolink = q;
        else
            (*node)->yeslink = q;
        q->parent = *node;
        q->yeslink = NULL;
        q->nolink = NULL;
        *node = q;
    }
    p = NULL;
    q = NULL;
    return true;
}

// -----

bool push(Node *pointer, Stack *stack)
{
    NodeForStack *temp = NULL;
    temp = (NodeForStack *)malloc(sizeof(NodeForStack));
    if(!temp)
        return false;
    temp->node = pointer;
    temp->link = *stack;

    *stack = temp;
    temp = NULL;
    return true;
}

void pop(Stack *stack)
{
    if(*stack)
    {
        NodeForStack *temp = *stack;
        *stack = (*stack)->link;
        temp->node = NULL;
        temp->link = NULL;
        free(temp);
        temp = NULL;
    }
}

bool isEmpty(Stack stack)
{
    return !stack;
}

bool onTop(Node **pointer, Stack stack)
{
    if(isEmpty(stack))
    {
        *pointer = stack->node;
        return true;
    }
    return false;
}

void clearStack(Stack *stack)
{
    while(!isEmpty(*stack))
        pop(stack);
}