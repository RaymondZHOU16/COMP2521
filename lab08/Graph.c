// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Pq.h"

// DO NOT modify this struct
struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
                    // 0 if nodes not adjacent
};

static bool validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(double));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

////////////////////////////////////////////////////////////////////////

int GraphNumVertices(Graph g) {
    return g->nV;
}

bool GraphInsertEdge(Graph g, struct edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);
    assert(e.weight > 0.0);

    if (g->edges[e.v][e.w] == 0.0) {
        g->edges[e.v][e.w] = e.weight;
        g->edges[e.w][e.v] = e.weight;
        g->nE++;
        return true;
    } else {
        return false;
    }
}

bool GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));

    if (g->edges[v][w] != 0.0) {   // edge e in graph
        g->edges[v][w] = 0.0;
        g->edges[w][v] = 0.0;
        g->nE--;
        return true;
    } else {
        return false;
    }
}

double GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));
    
    return g->edges[v][w];
}

void GraphShow(Graph g) {
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (int v = 0; v < g->nV; v++) {
        for (int w = v + 1; w < g->nV; w++) {
            if (g->edges[v][w] != 0.0) {
                printf("Edge %d - %d: %lf\n", v, w, g->edges[v][w]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
// Your task
struct edgeList {
    struct edge *edges;
    struct edgeList *next; 
};

// Sorts the edges of the given graph in ascending order of weight
Pq sortedEdgesSet(Graph g) {
    Pq pq = PqNew();
    if (pq == NULL) {
        fprintf(stderr, "edgeSet fail to create\n");
        exit(EXIT_FAILURE);
    }
    for (int v = 0; v < g->nV; v++) {
        for (int w = v + 1; w < g->nV; w++) {
            double weight = GraphIsAdjacent(g, v, w);
            if (weight > 0.0) {
                PqInsert(pq, (struct edge){v, w, g->edges[v][w]});
            }
        }
    }
    return pq;
}

// Find function using iterative approach to avoid deep nesting
int find(int parent[], int i) {
    while (parent[i] != -1) {
        i = parent[i];
    }
    return i;
}

// Union function to union two subsets
void unionSet(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) {
        parent[xset] = yset;
    }
}

//
static bool hasCycleHelper(Graph g, int i, int j, int parent[]) {
    if (g->edges[i][j] != 0.0) {
        int x = find(parent, i);
        int y = find(parent, j);
        if (x == y) {
            return true;
        }
        unionSet(parent, x, y);
    }
    return false;
}

// Function to check for a cycle in the graph using Union-Find
static bool hasCycle(struct graph *g) {
    bool hasCycle = false;
    int *parent = (int *)malloc(g->nV * sizeof(int));
    for (int i = 0; i < g->nV; ++i) {
        parent[i] = -1;
    }

    for (int i = 0; i < g->nV && !hasCycle; ++i) {
        for (int j = i + 1; j < g->nV && !hasCycle; ++j) {
            hasCycle = hasCycleHelper(g, i, j, parent);
        }
    }

    free(parent);
    return hasCycle;
}

Graph GraphMst(Graph g) {
    // TODO: Complete this function
 
    Pq edgesSet = sortedEdgesSet(g);
    Graph mst = GraphNew(g->nV);

    for (int i = 0; mst->nE < g->nV - 1; i++) {
        if (PqIsEmpty(edgesSet)) {
            PqFree(edgesSet);
            GraphFree(mst);
            return NULL;
        }
        struct edge e = PqExtract(edgesSet);
        GraphInsertEdge(mst, e);
        if (hasCycle(mst)) {
            GraphRemoveEdge(mst, e.v, e.w);
        }
    }
    PqFree(edgesSet);
    return mst;
}

////////////////////////////////////////////////////////////////////////

static bool validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

