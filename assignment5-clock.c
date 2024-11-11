#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Node
{
    int value;
    struct Node *prev;
    struct Node *next;
    bool secondChance;
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
    node->secondChance = false;
    return node;
}

void addNodeToList(LRUList *list, Node *node)
{
    if (!list->head)
    {
        // First node initialization
        list->head = list->tail = node;
        node->next = node->prev = node; // Circular link
    }
    else
    {
        // Insert node at the end (before tail)
        node->prev = list->tail;
        node->next = list->head;
        list->tail->next = node;
        list->head->prev = node;
        list->tail = node;
    }
    list->size++;
}

void replaceNode(LRUList *list, Node *hand, int newValue)
{
    while (hand->secondChance)
    {
        hand->secondChance = false;
        hand = hand->next;
    }
    // Replace the page at this position
    hand->value = newValue;
    hand->secondChance = true; // Set second chance on the new value
}

void printList(LRUList *list)
{
    Node *temp = list->head;
    if (!temp)
        return;

    int count = 0;
    do
    {
        printf("%d (%d) ", temp->value, temp->secondChance);
        temp = temp->next;
        count++;
    } while (temp != list->head && count < list->capacity);
    printf("\n");
}

int main()
{
    FILE *infile = fopen("Pages.txt", "r");
    if (infile == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char buffer[100];
    LRUList *list = (LRUList *)malloc(sizeof(LRUList));
    list->capacity = 3;
    list->size = 0;
    list->head = list->tail = NULL;

    Node *hand = NULL;

    while (fgets(buffer, sizeof(buffer), infile))
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (!isdigit(buffer[i]))
                continue;

            int pageValue = buffer[i] - '0';
            bool found = false;

            // Check if page is already in the list
            Node *temp = list->head;
            int count = 0;
            if (temp)
            {
                do
                {
                    if (temp->value == pageValue)
                    {
                        temp->secondChance = true; // Give a second chance
                        found = true;
                        break;
                    }
                    temp = temp->next;
                    count++;
                } while (temp != list->head && count < list->size);
            }

            if (!found)
            {
                if (list->size < list->capacity)
                {
                    // Add new page if there's still capacity
                    Node *newNode = createNode(pageValue);
                    addNodeToList(list, newNode);
                    if (!hand)
                        hand = newNode; // Initialize the hand pointer
                }
                else
                {
                    // Use the clock algorithm to replace a page
                    replaceNode(list, hand, pageValue);
                    hand = hand->next; // Move hand forward
                }
            }
            printList(list); // Print the list after each page reference
        }
    }

    // Free memory
    if (list->head)
    {
        Node *temp = list->head;
        do
        {
            Node *nextNode = temp->next;
            free(temp);
            temp = nextNode;
        } while (temp != list->head);
    }
    free(list);
    fclose(infile);

    return 0;
}