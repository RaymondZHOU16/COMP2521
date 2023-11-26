
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

int dayTrip(Graph g, Vertex s, Vertex vs[]) {
    // TODO
    int numDayTRip = 0;
    int *dist = malloc(g->nV * sizeof(int));

    // standard 
    for (int n = 0; n < g->nV; n++) {
        if (g->stdEdges[s][n] == true) {
            dist[n] = 1;
        }
    }

    // fast
    for (int n = 0; n <g->nV; n++) {
        if (g->fastEdges[s][n] == true) {
            dist[n] = 1;
            for (int i = 0; i < g->nV; i++) {
                if (g->fastEdges[n][i] == true) {
                    dist[i] = 1;
                }
            }
        }
    }

    for (int n = 0; n < g->nV ; n++) {
        if (dist[n] == 1 && (n != s)) {
            vs[numDayTRip] = n;
            numDayTRip++;
        }
    }
    free(dist);
    return numDayTRip;
}

