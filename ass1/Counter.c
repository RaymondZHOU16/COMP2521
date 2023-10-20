// Implementation of the Counter ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"

struct node {
    struct item data;
    struct node *left;
    struct node *right;
};

struct counter {
    struct node *root;
    int numItems;
};

// My Functions
// Helper function to create a new node
static struct node *createNode(char *token) {
    struct node *newNode = malloc(sizeof(struct node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->data.token = strdup(token);
    newNode->data.freq = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Helper function to free the nodes in the BST
static void freeNodes(struct node *root) {
    if (root == NULL) return;
    freeNodes(root->left);
    freeNodes(root->right);
    free(root->data.token);
    free(root);
}

// Helper function to add a token to the BST
static void addToken(struct node **root, char *token) {
    if (*root == NULL) {
        *root = createNode(token);
    } else {
        int cmp = strcmp(token, (*root)->data.token);
        if (cmp == 0) {
            (*root)->data.freq++;
        } else if (cmp < 0) {
            addToken(&((*root)->left), token);
        } else {
            addToken(&((*root)->right), token);
        }
    }
}

// Helper function to collect items into an array
static void collectItems(struct node *root, struct item *items, int *index) {
    if (root == NULL) return;
    collectItems(root->left, items, index);
	collectItems(root->right, items, index);
    items[*index] = root->data;
    (*index)++;
}

Counter CounterNew(void) {
    Counter counter = malloc(sizeof(struct counter));
    if (counter == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    counter->root = NULL;
    counter->numItems = 0;
    return counter;
}

void CounterFree(Counter c) {
    if (c == NULL) return;
    freeNodes(c->root);
    free(c);
}

void CounterAdd(Counter c, char *token) {
    addToken(&(c->root), token);
    // Increment numItems only when a new distinct token is added
    if (CounterGet(c, token) == 1) {
        c->numItems++;
    }
}

int CounterNumItems(Counter c) {
	return c->numItems;
}

int CounterGet(Counter c, char *token) {
    if (c == NULL || token == NULL) return 0;
    struct node *current = c->root;
    while (current != NULL) {
        int cmp = strcmp(token, current->data.token);
        if (cmp == 0) {
            return current->data.freq;
        } else if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return 0;
}

struct item *CounterItems(Counter c, int *numItems) {
    if (c == NULL || numItems == NULL) return NULL;
    struct item *items = malloc(sizeof(struct item) * c->numItems);
    if (items == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    collectItems(c->root, items, &index);
    *numItems = c->numItems;
    return items;
}

