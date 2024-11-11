// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <stdbool.h>
// #include <sys/types.h>
// #include <ctype.h>
// #include <fcntl.h>

// typedef struct Node
// {
//     int value;
//     struct Node *prev;
//     struct Node *next;
// } Node;

// typedef struct LRUList
// {
//     int capacity;
//     int size;
//     Node *head;
//     Node *tail;
// } LRUList;

// Node *createNode(int value)
// {
//     Node *node = (Node *)malloc(sizeof(Node));
//     node->value = value;
//     node->prev = node->next = NULL;
//     return node;
// }

// void swap(Node *node1, Node *node2)
// {
//     Node *temp = createNode(node1->value);
//     temp->next = node1->next;
//     temp->prev = node1->prev;

//     node1->next = node2->next;
//     node1->prev = node2->prev;
//     node1->value = node2->value;

//     node2->next = temp->next;
//     node2->prev = temp->prev;
//     node2->value = temp->value;
// }

// void printList(Node *head)
// {
//     Node *temp = head;
//     while (temp->next)
//     {
//         printf("%d, ", (int)temp->value);
//         temp = temp->next;
//     }
// }

// int main()
// {
//     int infile = open("Pages.txt", O_RDONLY);
//     int buffer[50];

//     read(infile, buffer, 50);

//     LRUList *list = (LRUList *)malloc(sizeof(LRUList));
//     list->capacity = 3;
//     list->size = 0;

//     Node *head = createNode(buffer[0] - '0');
//     Node *temp = head;

//     list->head = head;
//     list->tail = head;

//     for (int i = 0; i < 50; i++)
//     {
//         if (buffer[i] == 0)
//         {
//             break;
//         }
//         if (isdigit((char)buffer[i]))
//         {
//             Node *newNode = createNode((int)(buffer[i] - '0')); // converts the char to an int
//             if (list->size < list->capacity)
//             {
//                 // append new listnode onto the end of the list
//                 bool breakOut = false;
//                 while (temp)
//                 {
//                     // checks if the value is already in the list.
//                     if (temp->value == newNode->value)
//                     {
//                         swap(temp, list->head);
//                         list->head = temp;
//                         breakOut = true;
//                         break;
//                     }
//                     temp = temp->next;
//                 }
//                 if (breakOut)
//                 {
//                     continue;
//                 }
//                 temp = newNode;
//                 list->tail = newNode;
//             }
//             else
//             {
//                 // find where the new listnode should be placed in the program.
//                 bool breakOut = false;
//                 while (temp)
//                 {
//                     // checks if the value is already in the list.
//                     if (temp->value == newNode->value)
//                     {
//                         swap(temp, list->head);
//                         list->head = temp;
//                         breakOut = true;
//                         break;
//                     }
//                     temp = temp->next;
//                 }
//                 if (breakOut)
//                 {
//                     continue;
//                 }

//                 // remove tail, add newNode before head. list.head = newNode, list.tail = tail.prev
//                 list->tail = list->tail->prev;
//                 list->tail->next = NULL;

//                 newNode->next = list->head;
//                 list->head = newNode;
//             }
//         }
//         printList(list->head);
//     }
// }

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
