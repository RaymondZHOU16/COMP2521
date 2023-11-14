
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RoadMap.h"
#include "time.h"

struct roadMap {
	// TODO
	int nV;      // #vertices
	int nE;      // #edges
	int nR;      // #roads
	struct road **roads; // array of roads
};

static bool checkNodesMaxConnected(RoadMap map, int node);
static bool checkRoadInMap(RoadMap map, int node1, int node2);
static void addRoadToMap(RoadMap map, int node1, int node2, int travelMinutes);

////////////////////////////////////////////////////////////////////////
// Task 1

RoadMap RoadMapNew(int numNodes) {
	// TODO
	if (numNodes < 0) {
		fprintf(stderr, "error: invalid number of nodes\n");
		exit(EXIT_FAILURE);
	}

	RoadMap map = malloc(sizeof(*map));
	if (map == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	map->nV = numNodes;
	map->nE = 0;
	map->nR = 0;
	map->roads = NULL;

	return map;
}

void RoadMapFree(RoadMap map) {
	// TODO
	for (int i = 0; i < map->nE; i++) {
		free(map->roads[i]);
	}
	free(map->roads);
	free(map);
}

int RoadMapNumNodes(RoadMap map) {
	// TODO
	return map->nV;
}

int RoadMapNumRoads(RoadMap map) {
	// TODO
	return map->nR;
}

bool RoadMapAddRoad(RoadMap map, int node1, int node2,
                    bool isTwoWay, int travelMinutes) {
    // TODO
    // check if map is empty
    // check if already MAX_ROADS_PER_NODE roads connected to node1 or node2
    // check if there is already a road (one-way or two-way) between node1 and
    // node2
    if (map->roads == NULL || checkNodesMaxConnected(map, node1) ||
        checkNodesMaxConnected(map, node2) ||
        checkRoadInMap(map, node1, node2)) {
        // add road to map
        addRoadToMap(map, node1, node2, travelMinutes);
        if (isTwoWay) {
            addRoadToMap(map, node2, node1, travelMinutes);
        }
		map->nR++;
		printf("Road added from %d to %d\nTotal number of roads and "
				"edges is %d and %d\n",
				node1, node2, map->nR, map->nE);
		return true;
    }

    return false;
}

int RoadMapGetRoadsFrom(RoadMap map, int node, struct road roads[]) {
    // TODO
    int numRoads = 0;
	for (int i = 0; i < map->nE; i++) {
		if (map->roads[i]->fromNode == node) {
			roads[numRoads] = *(map->roads[i]);
			numRoads++;
		}
	}
    return numRoads;
}

int RoadMapGetRoadsTo(RoadMap map, int node, struct road roads[]) {
	// TODO
    int numRoads = 0;
	for (int i = 0; i < map->nE; i++) {
		if (map->roads[i]->toNode == node) {
			roads[numRoads] = *(map->roads[i]);
			numRoads++;
		}
	}
    return numRoads;
}

////////////////////////////////////////////////////////////////////////
// Task 2

void RoadMapProcessIslands(RoadMap map) {
	// TODO
}

int RoadMapNumIslands(RoadMap map) {
	// TODO
	return 0;
}

bool RoadMapOnSameIsland(RoadMap map, int node1, int node2) {
	// TODO
	return 0;
}

void RoadMapSetIslandName(RoadMap map, int node, char *name) {
	// TODO
}

char *RoadMapGetIslandName(RoadMap map, int node) {
	// TODO
	return "(unnamed)";
}

////////////////////////////////////////////////////////////////////////
// Task 3

void RoadMapAddTrafficLights(RoadMap map, int node) {
	// TODO
}

bool RoadMapHasTrafficLights(RoadMap map, int node) {
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////
// Task 4

void RoadMapSetClosedTimes(RoadMap map, int node1, int node2,
                           struct time from, struct time until) {
	// TODO
}

////////////////////////////////////////////////////////////////////////
// My helper functions

// check if there is not already MAX_ROADS_PER_NODE roads connected to node,
// return true if there is
static bool checkNodesMaxConnected(RoadMap map, int node) {
    int numRoads = 0;
    for (int i = 0; i < map->nE; i++) {
        if (map->roads[i]->fromNode == node || map->roads[i]->toNode == node) {
            numRoads++;
        }
    }
	printf("Number of roads connected to node %d is %d\n", node, numRoads);
    return numRoads < MAX_ROADS_PER_NODE;
}

// check if road is not already in map, return true if it is
static bool checkRoadInMap(RoadMap map, int node1, int node2) {
	for (int i = 0; i < map->nE; i++) {
		if ((map->roads[i]->fromNode == node1 && map->roads[i]->toNode == node2) ||
			(map->roads[i]->fromNode == node2 && map->roads[i]->toNode == node1)) {
			return false;
		}
	}
	return true;
}

// add road to map
static void addRoadToMap(RoadMap map, int node1, int node2, int travelMinutes) {
	struct road *newRoad = malloc(sizeof(struct road));
	if (newRoad == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	newRoad->fromNode = node1;
	newRoad->toNode = node2;
	newRoad->travelMinutes = travelMinutes;
	newRoad->closedFrom = (struct time){0, 0};
	newRoad->closedUntil = (struct time){0, 0};
	map->roads = realloc(map->roads, (map->nE + 1) * sizeof(*map->roads));
	if (map->roads == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	map->roads[map->nE] = newRoad;
	map->nE++;
}

