// Algorithms to design electrical grids

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Pq.h"
#include "place.h"

////////////////////////////////////////////////////////////////////////
// Your task

Graph cityGraph(struct place cities[], int numCities, struct place powerLine);
double distance(struct place p1, struct place p2);
int getPowerLine(Graph g, struct powerLine powerLines[], struct place cities[],
                 struct place powerPlant);
void addPowerLine(Graph g, int i, int j, struct place cities[],
                  struct place powerPlant, struct powerLine powerLines[],
                  int numLines);

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid1(struct place cities[], int numCities, struct place powerPlant,
              struct powerLine powerLines[]) {
    // TODO: Complete this function

    // Create a graph with numCities + 1 vertices
    Graph g = cityGraph(cities, numCities, powerPlant);

    // Find a minimum spanning tree of the graph
    Graph cityMst = GraphMst(g);

    // Store the power lines from graph
    int numPowerLines = getPowerLine(cityMst, powerLines, cities, powerPlant);

    // Free the graphs
    GraphFree(cityMst);
    GraphFree(g);

    return numPowerLines;
}

Graph cityGraph(struct place cities[], int numCities, struct place powerLine) {
    // Create a graph with numCities + 1 vertices
    Graph g = GraphNew(numCities + 1);

    // Add edges between the power plant and all cities
    for (int i = 0; i < numCities; i++) {
        struct edge e = {0, i + 1, distance(powerLine, cities[i])};
        GraphInsertEdge(g, e);
    }

    // Add edges between all cities
    for (int i = 0; i < numCities; i++) {
        for (int j = i + 1; j < numCities; j++) {
            struct edge e = {i + 1, j + 1, distance(cities[i], cities[j])};
            GraphInsertEdge(g, e);
        }
    }

    return g;
}

double distance(struct place p1, struct place p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Returns the power lines from the given graph
int getPowerLine(Graph g, struct powerLine powerLines[], struct place cities[],
                 struct place powerPlant) {
    int numLines = 0;

    // Iterate through all edges
    for (int i = 0; i < GraphNumVertices(g); i++) {
        for (int j = i + 1; j < GraphNumVertices(g); j++) {
            // If there is an edge between the two vertices, add the edge to the
            // power lines array
            addPowerLine(g, i, j, cities, powerPlant, powerLines, numLines);
            numLines++;
        }
    }

    return numLines;
}

// If there is an edge between the two vertices, add the edge to the power lines
// array
void addPowerLine(Graph g, int i, int j, struct place cities[],
                  struct place powerPlant, struct powerLine powerLines[],
                  int numLines) {
    if (GraphIsAdjacent(g, i, j) != 0) {
        if (i == 0) {
            struct powerLine line = {powerPlant, cities[j - 1]};
            powerLines[numLines] = line;
        } else {
            struct powerLine line = {cities[i - 1], cities[j - 1]};
            powerLines[numLines] = line;
        }
    }
}

////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(struct place cities[], int numCities, struct place powerPlants[],
              int numPowerPlants, struct powerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}
