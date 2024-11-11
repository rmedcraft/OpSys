#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>

typedef struct Node
{
    int value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct LRUList
{
    int capacity;
    int size;
    Node *head;
    Node *tail;
} LRUList;

Node *createNode(int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = value;
    node->prev = node->next = NULL;
    return node;
}

void moveToHead(LRUList *list, Node *node)
{
    if (node == list->head)
        return; // Node is already at head

    // Remove node from its current position
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;

    if (node == list->tail)
    {
        list->tail = node->prev;
    }

    // Move node to head
    node->next = list->head;
    node->prev = NULL;
    if (list->head)
        list->head->prev = node;
    list->head = node;

    if (!list->tail)
    {
        list->tail = node; // Set tail if the list was empty
    }
}

void removeTail(LRUList *list)
{
    if (!list->tail)
        return;

    Node *oldTail = list->tail;
    list->tail = list->tail->prev;
    if (list->tail)
    {
        list->tail->next = NULL;
    }
    else
    {
        list->head = NULL; // List is now empty
    }

    free(oldTail);
    list->size--;
}

void addToHead(LRUList *list, Node *node)
{
    node->next = list->head;
    node->prev = NULL;
    if (list->head)
        list->head->prev = node;
    list->head = node;

    if (!list->tail)
    {
        list->tail = node; // Set tail if the list was empty
    }

    list->size++;
}

void printList(Node *head)
{
    Node *temp = head;
    while (temp)
    {
        printf("%d ", temp->value);
        temp = temp->next;
    }
    printf("\n");
}

int main()
{
    // int infile = open("Pages.txt", O_RDONLY);
    FILE *infile = fopen("Pages.txt", "r");
    if (infile == NULL)
    {
        printf("Error opening file");
        return 1;
    }

    char buffer[100];

    buffer[99] = '\0';

    LRUList *list = (LRUList *)malloc(sizeof(LRUList));
    list->capacity = 3;
    list->size = 0;
    list->head = list->tail = NULL;
    while (fgets(buffer, sizeof(buffer), infile))
    {
        printf("Printing line\n");
        for (int i = 0; i < 100; i++)
        {
            if (!isdigit(buffer[i]))
                continue;

            int pageValue = buffer[i] - '0';
            Node *temp = list->head;
            bool found = false;

            while (temp)
            {
                if (temp->value == pageValue)
                {
                    moveToHead(list, temp);
                    found = true;
                    break;
                }
                temp = temp->next;
            }

            if (!found)
            {
                Node *newNode = createNode(pageValue);
                if (list->size == list->capacity)
                {
                    removeTail(list);
                }
                addToHead(list, newNode);
            }

            printList(list->head);
        }
        // clear list
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }

    // Free memory
    while (list->head)
    {
        Node *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    free(list);
    fclose(infile);
    return 0;
}
