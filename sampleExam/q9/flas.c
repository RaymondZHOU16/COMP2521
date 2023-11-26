
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct node *flas(struct node *l) {
    // TODO
    struct node *flas = NULL;
    int flasLen = 0;

    struct node *curr = l;
    while (curr != NULL) {
        struct node *start = curr;
        int len = 1;

        while (curr->next != NULL && curr->next->value > curr->value) {
            len++;
            curr = curr->next;
        }

        if (len > flasLen && len >= 2) {
            flas = start;
            flasLen = len;
        }
        curr = curr->next;
    }

    return flas;
}
