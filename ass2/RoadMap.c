
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RoadMap.h"
#include "time.h"

struct roadMap {
	// TODO
	int nN;      // number of nodes
	int nR;      // number of roads
	int nI;      // number of islands
	struct road **roads; // array of roads
	bool *isTwoWay; // array of booleans to indicate if road is two-way
	int *islandIds;    // array to store island ID for each node
    char **islandNames;    // array to store names of the islands
};

static bool checkMaxRoads(RoadMap map, int node);
static bool checkRoadExists(RoadMap map, int node1, int node2);
static void addRoad(RoadMap map, int node1, int node2, bool isTwoWay, int travelMinutes);
static void DFS(RoadMap map, int node, int islandID);

////////////////////////////////////////////////////////////////////////
// Task 1

RoadMap RoadMapNew(int numNodes) {
	// TODO
	if (numNodes < 0) {
		fprintf(stderr, "error: invalid number of nodes\n");
		exit(EXIT_FAILURE);
	}

	// initialise map
	RoadMap map = malloc(sizeof(*map));
	if (map == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	map->nN = numNodes;
	map->nR = 0;
	map->nI = 0;
	map->roads = NULL;
	map->isTwoWay = NULL;
	map->islandIds = NULL;
	map->islandNames = NULL;

	return map;
}

void RoadMapFree(RoadMap map) {
	// TODO
	for (int i = 0; i < map->nR; i++) {
		free(map->roads[i]);
	}
	for (int i = 0; i < map->nI; i++) {
		free(map->islandNames[i]);
	}
	free(map->roads);
	free(map->isTwoWay);
	free(map->islandIds);
	free(map->islandNames);
	free(map);
}

int RoadMapNumNodes(RoadMap map) {
	// TODO
	return map->nN;
}

int RoadMapNumRoads(RoadMap map) {
	// TODO
	return map->nR;
}

bool RoadMapAddRoad(RoadMap map, int node1, int node2,
                    bool isTwoWay, int travelMinutes) {
    // TODO
	if (map->roads == NULL) {
		// add first road
		addRoad(map, node1, node2, isTwoWay, travelMinutes);
    } else {
        // check if road already exists and if node has max roads
        if (checkRoadExists(map, node1, node2) || !checkMaxRoads(map, node1) ||
            !checkMaxRoads(map, node2)) {
            return false;
        }
		// add road
		addRoad(map, node1, node2, isTwoWay, travelMinutes);
	}
    return true;
}

int RoadMapGetRoadsFrom(RoadMap map, int node, struct road roads[]) {
    // TODO
	int numRoads = 0;
	for (int i = 0; i < map->nR; i++) {
		if (map->roads[i]->fromNode == node) {
			roads[numRoads] = *(map->roads[i]);
			numRoads++;
		} else if (map->roads[i]->toNode == node && map->isTwoWay[i]) {
			roads[numRoads].fromNode = node;
			roads[numRoads].toNode = map->roads[i]->fromNode;
			roads[numRoads].travelMinutes = map->roads[i]->travelMinutes;
			numRoads++;
		}
	}
    return numRoads;
}

int RoadMapGetRoadsTo(RoadMap map, int node, struct road roads[]) {
	// TODO
	int numRoads = 0;
	for (int i = 0; i < map->nR; i++) {
		if (map->roads[i]->toNode == node) {
			roads[numRoads] = *(map->roads[i]);
			numRoads++;
		} else if (map->roads[i]->fromNode == node && map->isTwoWay[i]) {
			roads[numRoads].fromNode = map->roads[i]->toNode;
			roads[numRoads].toNode = node;
			roads[numRoads].travelMinutes = map->roads[i]->travelMinutes;
			numRoads++;
		}
	}

	/*
	printf("node %d has %d roads connected\n", node, numRoads);
	for (int i = 0; i < numRoads; i++) {
		printf("road %d: %d -> %d\n", i, roads[i].fromNode, roads[i].toNode);
	}
	*/

    return numRoads;
}

////////////////////////////////////////////////////////////////////////
// Task 2

void RoadMapProcessIslands(RoadMap map) {
	// TODO
	// Initialize islandIds to -1, indicating unvisited nodes
    map->islandIds = malloc(sizeof(int) * map->nN);
    for (int i = 0; i < map->nN; i++) {
        map->islandIds[i] = -1;
    }

    int islandID = 0;

    // Run DFS for each node
    for (int i = 0; i < map->nN; i++) {
        if (map->islandIds[i] == -1) {
            DFS(map, i, islandID);
            islandID++;
        }
    }

	// Initialize islandNames to "(unnamed)"
	map->islandNames = malloc(sizeof(char *) * islandID);
	for (int i = 0; i < islandID; i++) {
		map->islandNames[i] = malloc(sizeof(char) * 10);
		strcpy(map->islandNames[i], "(unnamed)");
	}

    // Store the number of islands
    map->nI = islandID;

}

int RoadMapNumIslands(RoadMap map) {
	// TODO
	return map->nI;
}

bool RoadMapOnSameIsland(RoadMap map, int node1, int node2) {
	// TODO
	if (map->islandIds[node1] == map->islandIds[node2]) {
		return true;
	} else {
		return false;
	}
}

void RoadMapSetIslandName(RoadMap map, int node, char *name) {
	// TODO
	int islandID = map->islandIds[node];
	strcpy(map->islandNames[islandID], name);
}

char *RoadMapGetIslandName(RoadMap map, int node) {
	// TODO
	int islandID = map->islandIds[node];
	return map->islandNames[islandID];
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

// check if node has max roads
// returns true if node has less than max roads, false otherwise
static bool checkMaxRoads(RoadMap map, int node) {
	int nR = 0;
	for (int i = 0; i < map->nR; i++) {
		if (map->roads[i]->fromNode == node || map->roads[i]->toNode == node) {
			nR++;
		}
	}
	if (nR == MAX_ROADS_PER_NODE) {
		return false;
	}
	//printf("//checking node %d has %d roads connected\n", node, nR);
	return true;
}

// check if road is already in map, regardless of direction
// returns true if road exists, false otherwise
static bool checkRoadExists(RoadMap map, int node1, int node2) {
	for (int i = 0; i < map->nR; i++) {
		if ((map->roads[i]->fromNode == node1 && map->roads[i]->toNode == node2) || 
			(map->roads[i]->fromNode == node2 && map->roads[i]->toNode == node1)) {
			return true;
		}
	}
	return false;
}

// add road to map
static void addRoad(RoadMap map, int node1, int node2, bool isTwoWay, int travelMinutes) {
	map->nR++;
	map->roads = realloc(map->roads, map->nR * sizeof(*(map->roads)));
	if (map->roads == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	map->roads[map->nR - 1] = malloc(sizeof(struct road));
	if (map->roads[map->nR - 1] == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	map->roads[map->nR - 1]->fromNode = node1;
	map->roads[map->nR - 1]->toNode = node2;
	map->roads[map->nR - 1]->travelMinutes = travelMinutes;
	map->isTwoWay = realloc(map->isTwoWay, map->nR * sizeof(bool));
	if (map->isTwoWay == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	map->isTwoWay[map->nR - 1] = isTwoWay ? true : false;
}

// DFS to find islands
static void DFS(RoadMap map, int node, int islandID) {
	map->islandIds[node] = islandID;
	struct road roads[MAX_ROADS_PER_NODE];
	int numRoads = RoadMapGetRoadsFrom(map, node, roads);
	for (int i = 0; i < numRoads; i++) {
		if (map->islandIds[roads[i].toNode] == -1) {
			DFS(map, roads[i].toNode, islandID);
		}
	}
}