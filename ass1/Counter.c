// Implementation of the Counter ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"

struct node {
    struct item item;
    struct node *left;
    struct node *right;
};

struct counter {
    struct node *root;
};

Counter CounterNew(void) {
    Counter c = malloc(sizeof(struct counter));
    if (c == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    c->root = NULL;
    return c;
}

static void freeNode(struct node *n) {
    if (n == NULL) {
        return;
    }
    freeNode(n->left);
    freeNode(n->right);
    free(n->item.token);
    free(n);
}

void CounterFree(Counter c) {
    if (c == NULL) {
        return;
    }
    freeNode(c->root);
    free(c);
}

static struct node *addNode(struct node *n, char *token) {
    if (n == NULL) {
        n = malloc(sizeof(struct node));
        if (n == NULL) {
            fprintf(stderr, "Error: out of memory\n");
            exit(EXIT_FAILURE);
        }
        n->item.token = strdup(token);
        n->item.freq = 1;
        n->left = NULL;
        n->right = NULL;
    } else if (strcmp(token, n->item.token) < 0) {
        n->left = addNode(n->left, token);
    } else if (strcmp(token, n->item.token) > 0) {
        n->right = addNode(n->right, token);
    } else {
        n->item.freq++;
    }
    return n;
}

void CounterAdd(Counter c, char *token) {
    if (c == NULL || token == NULL) {
        return;
    }
    c->root = addNode(c->root, token);
}

static int countNodes(struct node *n) {
    if (n == NULL) {
        return 0;
    }
    return 1 + countNodes(n->left) + countNodes(n->right);
}

int CounterNumItems(Counter c) {
    if (c == NULL) {
        return 0;
    }
    return countNodes(c->root);
}

static int getNodeCount(struct node *n, char *token) {
    if (n == NULL) {
        return 0;
    }
    if (strcmp(token, n->item.token) < 0) {
        return getNodeCount(n->left, token);
    } else if (strcmp(token, n->item.token) > 0) {
        return getNodeCount(n->right, token);
    } else {
        return n->item.freq;
    }
}

int CounterGet(Counter c, char *token) {
    if (c == NULL || token == NULL) {
        return 0;
    }
    return getNodeCount(c->root, token);
}

static void copyItem(struct item *dest, struct item *src) {
    dest->token = strdup(src->token);
    dest->freq = src->freq;
}

static void fillArray(struct node *n, struct item *items, int *index) {
    if (n == NULL) {
        return;
    }
    fillArray(n->left, items, index);
    copyItem(&items[*index], &n->item);
    (*index)++;
    fillArray(n->right, items, index);
}

struct item *CounterItems(Counter c, int *numItems) {
    if (c == NULL || numItems == NULL) {
        return NULL;
    }
    struct item *items = malloc(CounterNumItems(c) * sizeof(struct item));
    if (items == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    fillArray(c->root, items, &index);
    *numItems = CounterNumItems(c);
    return items;
}