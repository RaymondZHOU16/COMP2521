
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

int maxDegreeDiff(Graph g) {
    int *diff = calloc(g->nV, sizeof(int));

    for (int i = 0; i < g->nV; i++) {
        struct adjNode *curr;
        for (curr = g->edges[i]; curr != NULL; curr = curr->next) {
            diff[i]++;
            diff[curr->v]--;
        }
    }

    int maxDiff = 0;
    for (int i = 0; i < g->nV; i++) {
        if (abs(diff[i]) > maxDiff) {
            maxDiff = abs(diff[i]);
        }
    }
    
    free(diff);
    return maxDiff;
}

